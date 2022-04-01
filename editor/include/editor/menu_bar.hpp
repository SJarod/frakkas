#pragma once

namespace Editor
{
    class MenuBar
    {
    public:
        MenuBar() = default;
        ~MenuBar() = default;

        /**
        * @summary Display the ImGui panel
        */
        void OnImGuiRender();
        void FrakkasColors();

    private:
        void FileField();
        void EditField();
        void OptionsField();
    };
}