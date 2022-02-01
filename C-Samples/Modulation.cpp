#include <iostream>
#include <vector>

class Modulator
{
public:
    float max;
    float min;
    float freq;
    float delta;
    std::vector<float> wave;
    bool enable;
    Modulator(float max_r, float min_r, float freq_r, float delta_r)
    {
        max = max_r;
        min = min_r;
        // if (freq_r * 4 <= 1 / delta_r)
        //{
        freq = freq_r;
        delta = delta_r;
        enable = true;
        buildWave();
        // }
        /*else
        {
            std::cout << "Time step two low for given frequency." << std::endl;
            enable = false;
        }*/
    };

    float nextValue()
    {
        // std::cout << wave.back() << std::endl;
        int size = wave.size();
        if (iterator >= size)
        {
            iterator = 0;
        }
        float aux = wave[iterator];
        iterator++;
        return aux;
    };

private:
    int iterator;
    void buildWave()
    {
        float mean = (max + min) / 2;
        for (float i = 0; i < 1 / (freq); i += delta)
        {
            if (i < 0.25 / (freq))
            {
                wave.push_back((max - mean) / (0.25 / (freq)) * i + mean);
            }
            else if (i < 0.75 / (freq))
            {
                wave.push_back((min - max) / (0.5 / (freq)) * i + (max - min));
            }
            else if (i <= 1 / (freq))
            {
                wave.push_back((mean - min) / (0.25 / (freq)) * i + 4 * (min - mean));
            }
        }
    };
};

int main(int argc, char const *argv[])
{
    Modulator mod(-1, 1, 20, 0.00024);
    std::cout << "Here" << std::endl;
    for (int i = 0; i < 300; i++)
    {
        std::cout << mod.nextValue() << std::endl;
    }
    return 0;
}
