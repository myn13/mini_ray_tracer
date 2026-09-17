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

# Phase 2:
## Random
- Under multithreading, random is sharing the glocal state so it will crash the system when threads call random functions.
- Fix:
```cpp
inline random_generator& get_thread_rng() {
    thread_local random_generator rng;
    return rng;
}

inline double random_less_than_1() {
    return get_thread_rng().next_double();
}
```
- random_generator class uses engine mt19937 and uniform distribution to generate a random number from 0 to 1.0.
## Tiles
- Each thread will render a tile of image. The tiles are stored in an array and kept track by index.

# Phase 3:
## AABB needs
- A constructor/representation: min_corner, max_corner. This constructor needs to check the given max and min cornner and rearrange them as needed.
- A hit() method: the slab test, ray-vs-this-box, returning bool within a t_min/t_max range
- A way to combine two boxes: either a free function or a constructor that takes two aabbs and produces their union — needed for building parent nodes during tree construction
- A padding to handle zero-thickness degenerate cases
- longest axis -> it is used to decide which axis direction should be splitted. 

## Build a bvh tree
```
build_bvh_node(object_list):
    compute the bounding box enclosing all objects in the list
    axis = that box's longest_axis()
    sort the object list by each object's center position along that axis
    split the sorted list in half
    left_child  = build_bvh_node(first half)     # recursive
    right_child = build_bvh_node(second half)    # recursive
```

## Edit the object on image