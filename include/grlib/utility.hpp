/** @file */
#pragma once

#include <random>
#include <stdexcept>

int randomize(int min,int max);

struct Color_pool {
        using Color = std::pair<std::string, bool>;

        /**
         * Initialize pool with a range of predefined colors
         */
        Color_pool()
        : used(0UL)
        {
                colors.emplace_back("#91C4F2", false);
                colors.emplace_back("#8CA0D7", false);
                colors.emplace_back("#9D79BC", false);
                colors.emplace_back("#A14DA0", false);
                colors.emplace_back("#7E1F86", false);
                colors.emplace_back("#B4EDD2", false);
                colors.emplace_back("#A0CFD3", false);
                colors.emplace_back("#8D94BA", false);
                colors.emplace_back("#9A7AA0", false);
                colors.emplace_back("#87677B", false);
                colors.emplace_back("#FE938C", false);
                colors.emplace_back("#E6B89C", false);
                colors.emplace_back("#EAD2AC", false);
                colors.emplace_back("#9CAFB7", false);
                colors.emplace_back("#4281A4", false);
                colors.emplace_back("#0B3954", false);
                colors.emplace_back("#087E8B", false);
                colors.emplace_back("#BFD7EA", false);
                colors.emplace_back("#FF5A5F", false);
                colors.emplace_back("#C81D25", false);
                colors.emplace_back("#A491D3", false);
                colors.emplace_back("#818AA3", false);
                colors.emplace_back("#C5DCA0", false);
                colors.emplace_back("#F5F2B8", false);
                colors.emplace_back("#F9DAD0", false);
                colors.emplace_back("#0D3B66", false);
                colors.emplace_back("#FAF0CA", false);
                colors.emplace_back("#F4D35E", false);
                colors.emplace_back("#EE964B", false);
                colors.emplace_back("#F95738", false);
        }


        /**
         * Function returns next not used color
         * @return string containing color
         * Throws the exception if all colors are used already
         */
        std::string next_color()
        {
                for (auto& color : colors)
                        if (!color.second) {
                                color.second = true;
                                used++;
                                return color.first;
                        }

                throw std::runtime_error("No colors available.");
        }

        /**
         * Function returns any color
         * @return string containing color
         */
        std::string random_color()
        {
                int val = randomize(0, colors.size() - 1);
                colors[val].second = true;
                used++;
                return colors[val].first;
        }

        /**
         * Function returns any unique color
         * @return string containing color
         * Throws the exception if all colors are used already
         */
        std::string random_unique_color()
        {
                if (used == colors.size())
                        throw std::runtime_error("No colors available.");

                while (true) {
                        int val = randomize(0, colors.size() - 1);

                        if (colors[val].second)
                                continue;

                        colors[val].second = true;
                        used++;
                        return colors[val].first;
                }
        }

        std::vector<Color> colors;
        size_t used;
};

