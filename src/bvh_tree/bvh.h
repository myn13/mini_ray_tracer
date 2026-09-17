#ifndef BVH_H
#define BVH_H
#include "aabb.h"
#include "geometry/hittable_list.h"
inline void sort_objects_by_center(std::vector<std::shared_ptr<hittable>>& objects, int axis, size_t start, size_t end) {
    std::sort(objects.begin() + start, objects.begin() + end, [axis](const std::shared_ptr<hittable>& a, const std::shared_ptr<hittable>& b) {
        point3 center_a = a->bounding_box().get_center();
        point3 center_b = b->bounding_box().get_center();
        return center_a[axis] < center_b[axis];
    });
}
class bvh_node : public hittable {
    private:
        std::shared_ptr<hittable> left;
        std::shared_ptr<hittable> right;
        aabb box;
    public:
        bvh_node(std::vector<std::shared_ptr<hittable>>& world, size_t start, size_t end) {
            // std::cout << "Building bvh_node: start=" << start << " end=" << end << "\n";
            aabb combined_box;
            for (size_t i = start; i < end; ++i) {
                combined_box = combine(combined_box, world[i]->bounding_box());
            }
            int longest_axis = combined_box.longest_axis();
            sort_objects_by_center(world, longest_axis, start, end);
            size_t count = end - start;
            if (count == 1) left = right = world[start];
            else if (count == 2) {
                left = world[start];
                right = world[start + 1];
            } else {
                size_t mid = start + count / 2;
                // std::cout << "  count=" << count << " mid=" << mid << "\n";
                left = std::make_shared<bvh_node>(world, start, mid);
                right = std::make_shared<bvh_node>(world, mid, end);
            }
            box = combine(left->bounding_box(), right->bounding_box());
        }
        bvh_node(hittable_list& world) : bvh_node(world.objects, 0, world.objects.size()) {}
        bool get_hit(const ray &r, double t_max, double t_min, hit_record &rec) const override {
            if (!box.get_hit(r, t_max, t_min)) return false;
            bool hit_left = left->get_hit(r, t_max, t_min, rec);
            bool hit_right = right->get_hit(r, hit_left ? rec.t : t_max, t_min, rec);
            return hit_left || hit_right;
        }

        virtual aabb bounding_box() const override { return box;}
};
#endif