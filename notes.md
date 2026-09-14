# Phase 0:
- I made vec3 using double data type because I want the tracer to be more correct than faster. It is the trade-off because float is half memory than double but more accurate. 
- Core math: vec3, color, ray 
    - I made color class seperating from vec3 because I want to render color faster instead of calling vector alias 
    - I define ray(t) = origin + t * direction

# Phase 1:
## Deliveries:
- Hittable interface and 3 shapes:
    - Hittable interface will have virtual deconstruction and get_hit which is implemented by each shape class.
        - t_max and t_min are the valid window along the ray that you are willing to accept an intersection as real. 
    - Sphere:
        - get_hit defines which t is the intersection of ray and sphere. 
    - Cube: the cube is an axis_aligned box.
        - get_hit defined by the boundary of x, y, and z.
        - Call intersect point P = a + bt
        - for x axis, x_min <= a + bt <= x+max
        - Solve the inequation to get t0 <= t <= t1
        - Same for y ans z axis
        - t is the max of all lower boundary of t in all axis. 
- Hittable list
- Camera
    - Location is at the top left pixel, we need to move half each delta u and v to get the location at the center of pixel
    ```cpp 
    point3 pixel_00_at = viewport_upper_left + 0.5 * pixel_delta_u + 0.5 * pixel_delta_v;
    ```
    - To decide the color of pixel, there are three cases happening:
        - Ray hits nothing: return background color
        - Ray hits the object and the object can reflect: the ray bounces. It can bounce infinity if objects around are diffuse so we need max depth which decides when we stop the recursion of generating ray color.
        - The render loop itself:
            Nested loop over image rows/columns
            Multiple samples per pixel (antialiasing) — averaging jittered rays per pixel
            Recursive ray_color() function: trace ray → hit? → ask material to scatter → recurse (with a max-depth cutoff to prevent infinite recursion) → or return background color if no hit
            Output via write_color
- Material: lambertian, metal, dielectric

