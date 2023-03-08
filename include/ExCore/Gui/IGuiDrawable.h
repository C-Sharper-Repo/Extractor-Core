#pragma once
#include <imgui.h>

namespace ExCore {
    template<typename E>
    struct IGuiMeta {
    public:
        E& getExtraMeta() { return _extra; }
    private:
        E _extra{};
    };

    template<typename T>
    struct IGuiDrawable {
        static bool onGui(const char* label, T& value, const bool doInline = false) {
            return false;
        }

        static bool onGui(T& value, const bool doInline = false) {
            return onGui(nullptr, value, doInline);
        }

    };

    namespace Gui {
        template<typename T>
        static bool drawGui(T& value) {
            return IGuiDrawable<T>::onGui(value);
        }

        template<typename T>
        static bool drawGui(const char* label, T& value) {
            return IGuiDrawable<T>::onGui(label, value);
        }

        template<typename T>
        static bool drawGuiInline(T& value) {
            return IGuiDrawable<T>::onGui(value, true);
        }

        template<typename T>
        static bool drawGuiInline(const char* label, T& value) {
            return IGuiDrawable<T>::onGui(label, value, true);
        }
    }
}