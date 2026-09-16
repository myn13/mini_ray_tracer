#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H
#include "hittable.h"
#include "core/vec3.h"
class hittable_list: public hittable {
    public:
        std::vector <std::shared_ptr<hittable>> objects;
        hittable_list(){};
        explicit hittable_list(std::shared_ptr<hittable> object) {
            add(object);
        }
        void add(std::shared_ptr<hittable> object) {
            objects.push_back(object);
        }
        void clear() {
            objects.clear();
        }
        virtual bool get_hit(const ray &r, double t_max, double t_min, hit_record &record) const override {
            bool hit_any = false;
            double closest_hit = t_max;
            hit_record temp;
            for (const auto& o: objects) {
                if (o->get_hit(r, t_max, t_min, temp)) {
                    hit_any = true;
                    closest_hit = temp.t;
                    record = temp;
                }
            }
            return hit_any;
        }
        virtual aabb bounding_box() const override {
            aabb box;
            for (const auto& o: objects) {
                box = combine(box, o->bounding_box);
            }
            return box;
        }
}; 
#endif