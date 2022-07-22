#include <string>

#define nodepend UINT32_MAX
#define opt_default(T, V) .has_default = true, .T = V

struct ConfigOption
{
    std::string name;
    enum
    {
        BOOL,
        INT,
        FLOAT,
        STRING
    } type;
    size_t offset;
    size_t depends{ nodepend };
    bool has_default{ false };
    union
    {
        bool b;
        float f;
        int i;
        const char* s;
    };
};

bool parse_custom(ConfigOption options[], size_t size, Config* config);