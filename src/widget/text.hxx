//
// Created by kenym on 08/02/24.
//

#ifndef GTKPOLY_TEXT_HXX
#define GTKPOLY_TEXT_HXX

#include <gtkmm/text.h>
#include <memory>

#include "../messages/widgets/text.np.hxx"

namespace Poly
{
    class Text final : public Gtk::Text
    {
    public:
        int32_t tag;
    };

    std::unique_ptr<Text> make_text(const Message::Text& msg);
} // namespace Poly

#endif // GTKPOLY_TEXT_HXX
