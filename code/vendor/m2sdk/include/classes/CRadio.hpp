#pragma once

namespace M2
{
    class ICRadio
    {
    public:
    };

    class C_Radio : public ICRadio
    {
    public:
        void ChangePlaylist(const char *playlist)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0xCD25B0, this, playlist);
        }

        void ChangeProgram(const char *program)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0xCD2570, this, program);
        }

        void ChangeStation(const char *station)
        {
            Mem::InvokeFunction<Mem::call_this, void>(0xCD8AE0, this, station);
        }

        void SetStatusOff()
        {
            Mem::InvokeFunction<Mem::call_this, bool>(0xCCF0C0, this);
        }

        void SetStatusOnFromScript()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xCD8BD0, this);
        }

        void SetStatusOnInternal()
        {
            Mem::InvokeFunction<Mem::call_this, int>(0xCD8550, this);
        }
    };
};
