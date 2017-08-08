#include <stdio.h>
#include <librg/streamer.h>

namespace unit
{
    void streamer_test()
    {
        describe("streamer.h", [](case_t it) {

            librg::entities_initialize();
            librg::streamer_initialize();

            auto entity = librg::entities->create();
            entity.assign<librg::streamable_t>(hmm_vec3{ 1000, 1000, 1000 });
            entity.assign<librg::transform_t>(hmm_vec3{ 30,20,0 });

            //librg::streamer::insert(entity);

            it("should be able to return queue", [entity](vald_t validate) {

                auto queue = librg::streamer::query(entity);

                validate(queue.size() == 0);
            });

            it("should be able to return exactly 1 entity", [entity](vald_t validate) {
                librg::streamer::clear();

                auto friendly = librg::entities->create();
                friendly.assign<librg::streamable_t>(hmm_vec3{ 1000, 1000, 1000 });
                friendly.assign<librg::transform_t>(hmm_vec3{ 30,20,10 });

                librg::streamer::insert(friendly);

                auto queue = librg::streamer::query(entity);

                validate(queue.size() == 1);
            });

            librg::streamer::clear();

            it("should be able to return exactly 666 entities", [entity](vald_t validate) {
                for (int i = 0; i < 666; i++) {
                    auto enemy = librg::entities->create();
                    enemy.assign<librg::streamable_t>(hmm_vec3{ 300, 300, 300 });
                    enemy.assign<librg::transform_t>(hmm_vec3{ (float)i,20,10 });

                    librg::streamer::insert(enemy);
                }

                auto queue = librg::streamer::query(entity);

                validate(queue.size() == 666);
            });

            librg::streamer::clear();

            it("should be able to return less than 32k entities", [entity](vald_t validate) {
                auto newEntity = librg::entities->create();
                newEntity.assign<librg::streamable_t>(hmm_vec3{ 30000, 30000, 30000 });
                newEntity.assign<librg::transform_t>();
                for (int i = 0; i < 48000; i++) {
                    auto enemy = librg::entities->create();
                    enemy.assign<librg::streamable_t>(hmm_vec3{ 300, 300, 300 });
                    auto ft = librg::transform_t();
                    ft.position = hmm_vec3{ (float)i,30.f,10 };
                    enemy.assign<librg::transform_t>(ft);

                    librg::streamer::insert(enemy);
                }
                auto queue = librg::streamer::query(newEntity);
                validate(queue.size() <= 32000);
            });

            librg::streamer::clear();

            it("should be able to blacklist 1 entity globally", [entity](vald_t validate) {
                auto badEntity = librg::entities->create();
                badEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
                badEntity.assign<librg::transform_t>();
                librg::streamer::set_visible(badEntity, false);

                auto goodEntity = librg::entities->create();
                goodEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
                goodEntity.assign<librg::transform_t>();

                librg::streamer::insert(badEntity);
                librg::streamer::insert(goodEntity);

                auto queue = librg::streamer::query(entity);
                validate(queue.size() == 1);
            });

            librg::streamer::clear();

            it("should be able to ignore 1 entity for target entity", [entity](vald_t validate) {
                auto badEntity = librg::entities->create();
                badEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
                badEntity.assign<librg::transform_t>();
                //librg::streamer::set_visible(badEntity, false);

                auto goodEntity = librg::entities->create();
                goodEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
                goodEntity.assign<librg::transform_t>();

                auto targetEntity = librg::entities->create();
                targetEntity.assign<librg::streamable_t>(hmm_vec3{ 100, 100, 100 });
                targetEntity.assign<librg::transform_t>();

                librg::streamer::insert(badEntity);
                librg::streamer::insert(goodEntity);
                librg::streamer::insert(targetEntity);

                librg::streamer::set_visible_for(targetEntity, badEntity, false);

                auto queue = librg::streamer::query(targetEntity);
                auto biggerQueue = librg::streamer::query(goodEntity);
                validate(queue.size() == 1 && biggerQueue.size() == 2);
            });
        });
    }
}
