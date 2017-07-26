#include <librg/utils/fs.h>

namespace unit
{
    void fs_test() {
        describe("utils/fs.h", [](case_t it) {
            using namespace librg;

            it("should create folder", [](vald_t validate) {
                int result = fs::mkdir("testdir");

                validate(result == 0 || result == UV_EEXIST);
            });

            // it("should be able to read file content async", [](vald_t validate) {
            //     fs::read("", [validate](fs::result_t* res) {
            //         validate(true);
            //     });
            // });

            std::string testString = "Hello World!";
            std::string testFileName = "test_fs.txt";

            it("should write text to file", [&](vald_t validate) {
                int result = fs::write(testFileName, testString.size(), (fs::byte*)testString.c_str(), [validate](fs::result_t *res) { });

                validate(result == 0);
            });

            it("should read file content", [&](vald_t validate) {
                fs::read(testFileName, [validate, testString](fs::result_t *res) {
                    validate(testString == std::string(res->content));
                });
            });
        });
    }
}
