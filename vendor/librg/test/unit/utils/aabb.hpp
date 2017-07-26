#include <librg/utils/aabb.h>

namespace unit
{
    void aabb_test() {
        describe("utils/aabb.h", [](case_t it) {
            using namespace librg;

            auto pinside  = hmm_vec3{ 2, 2, 2 };
            auto poutside = hmm_vec3{ 10, 2, 2 };

            it("should be able to create itself", [](vald_t validate) {
                auto sample = aabb_t(hmm_vec3{5, 5, 5});
                validate((sample.halfdim.X == 5));
            });

            it("should be able to check if 2d point is inside", [pinside](vald_t validate) {
                validate(aabb_t(hmm_vec3{5, 5, 5}).contains_2d(pinside));
            });

            it("should be able to check if 2d point is outside", [poutside](vald_t validate) {
                validate(!aabb_t(hmm_vec3{5, 5, 5}).contains_2d(poutside));
            });

            it("should be able to check if 3d point is inside", [pinside](vald_t validate) {
                validate(aabb_t(hmm_vec3{5, 5, 5}).contains_3d(pinside));
            });

            it("should be able to check if 3d point is outside", [poutside](vald_t validate) {
                validate(!aabb_t(hmm_vec3{5, 5, 5}).contains_3d(poutside));
            });
        });
    }
}
