int main()
{
    bool running = true;
    int foo = 1;
    while(running)
    {
        /// NEW FRAME

        /// UPDATE

        /// ENDFRAME

        if (foo++ == 3)
            running = false;
    }
    return 0;
}