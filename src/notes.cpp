#include <Arduino.h>

class Notes {
    private:
        int32_t noteSteps[8][12] = {
            {6384615,6764085,7166251,7592575,8044035,8522338,9028949,9566064,10134659,10737418,11375807,12052264},
            {12769231,13528171,14332501,15185150,16088069,17044675,18057897,19132127,20269317,21474836,22751613,24104528},
            {25538461,27056342,28665002,30370300,32176138,34089351,36115795,38264254,40538635,42949673,45503226,48209056},
            {51076922,54112683,57330004,60740599,64352276,68178701,72231589,76528508,81077269,85899345,91006452,96418111},
            {102153844,108225366,114660008,121481198,128704552,136357402,144463178,153057016,162154538,171798690,182012904,192836222},
            {204307688,216450732,229320016,242962396,257409104,272714804,288926356,306114032,324309076,343597380,364025808,385672444},
            {408615376,432901464,458640032,485924792,514818208,545429608,577852712,612228064,648618152,687194760,728051616,771344888},
            {817230752,865802928,917280064,971849584,1029636416,1090859216,1155705424,1224456128,1297236304,1374389520,1456103232,1542689776}
        };

        String notes[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    public:
        int32_t getStep(int octave, int note) {
            return noteSteps[octave][note];
        }

        String getNote(int note) {
            return notes[note];
        }
};