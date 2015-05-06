#include "parse.h"

std::string tocss(const std::string &str)
{
    std::string ans = "<head><link href=\""
                + str
                + "\" rel=\"stylesheet\" type=\"text/css\"></head>";
    return ans;
}

std::string tohtml(const std::string &str)
{
    struct buf *ib, *ob;

    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;

    /* reading everything */
    ib = bufnew(READ_UNIT);
    bufgrow(ib, READ_UNIT);

    int l = str.length();
    int i = 0;
    for (i = 0; i < l; ++i)
    {
        *(ib->data + ib->size) = str[i];
        ib->size += 1;
        bufgrow(ib, ib->size + READ_UNIT);
    }

    /* performing markdown parsing */
    ob = bufnew(OUTPUT_UNIT);

    sdhtml_renderer(&callbacks, &options, 0);
    markdown = sd_markdown_new(0, 16, &callbacks, &options);

    sd_markdown_render(ob, ib->data, ib->size, markdown);
    sd_markdown_free(markdown);

    std::string ans((char *)ob->data);

    /* cleanup */
    bufrelease(ib);
    bufrelease(ob);
    return (ans);
}

std::string tofinalhtml(const std::string &css, const std::string &content)
{
    //std::string ans = "<!doctype html>\n<html>\n<head><link href=\""+ css + "\" rel=\"stylesheet\" type=\"text/css\"></head>\n<body>" + tohtml(content) + "</body>\n</html>";
    std::string ans = "<!doctype html>\n<html>\n<head></head>\n<body>" + tohtml(content) + "</body>\n <style>\n"+ css + "\n</style>\n</html>";
    return ans;
}
