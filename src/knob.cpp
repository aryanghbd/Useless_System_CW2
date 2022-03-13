#include <atomic>

class Knob {
    public:
        std::atomic<int> knobRotation;
        int rotationMin;
        int rotationMax;
        uint8_t preBA;
        uint8_t currBA;
        uint8_t preBAcurrBA;
        int previousRotation;
        int rotationIncrement;

        Knob(int rotationIncrement=1,
            int rotationMin=0,
            int rotationMax=16,
            int knobRotation = 0)
            {
                this->rotationMin = rotationMin;
                this->rotationMax = rotationMax;

                this->knobRotation = 0;
                this->preBA = 0;
                this->currBA = 0;
                this->preBAcurrBA = 0;
                this->previousRotation = 0;
                this->rotationIncrement = 1;
        }

        void addKnobRotation (int n) {
            if (this->knobRotation + n >= rotationMax) {
                this->knobRotation = rotationMax;
            } else if (this->knobRotation + n <= rotationMin) {
                this->knobRotation = rotationMin;
            } else {
                this->knobRotation += n;
            }
            this->previousRotation = n;
        }

        void setCurrBA (uint8_t ba) {
            this->preBA = this->currBA;
            this->currBA = ba;
            this->preBAcurrBA = (this->preBA << 2) + ba;
        }

        void updateRotation() {
            if (this->preBAcurrBA == 0b0011 
                    || preBAcurrBA == 0b0110
                    || preBAcurrBA == 0b1001
                    || preBAcurrBA == 0b1100) 
                    {
                        this->addKnobRotation(previousRotation);
                    }
            else if (preBAcurrBA == 0b0001
                    || preBAcurrBA == 0b0111
                    || preBAcurrBA == 0b1000
                    || preBAcurrBA == 0b1110) 
                    {
                        this->addKnobRotation(-this->rotationIncrement);
                    }
            else if (preBAcurrBA == 0b0010
                    ||preBAcurrBA == 0b0100
                    ||preBAcurrBA == 0b1011
                    ||preBAcurrBA == 0b1101) 
                    {
                        this->addKnobRotation(this->rotationIncrement); 
                    }
        }
    
        int getRotation() {
            return this->knobRotation;
        }
};

