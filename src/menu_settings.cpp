#include "menu_settings.hpp"
#include "nlohmann/json.hpp"

namespace big
{
    void menu_settings::attempt_save()
    {
        nlohmann::json j = *this;

        if (deep_compare(this->options, j, true))
            this->save();
    }

    bool menu_settings::load()
    {
        this->default_options = *this;

        std::string settings_file = std::getenv("appdata");
        settings_file += this->settings_location;

        std::ifstream file(settings_file);

        if (!file.is_open())
        {
            this->write_default_config();

            file.open(settings_file);
        }

        try
        {
            file >> this->options;
        }
        catch (const std::exception&)
        {
            LOG(HACKER) << "Detected corrupt settings, writing default config...";

            this->write_default_config();

            return this->load();
        }

        bool should_save = this->deep_compare(this->options, this->default_options);

        from_json(this->options, *this);

        if (should_save)
        {
            LOG(HACKER) << "Updating settings...";
            save();
        }

        return true;
    }
    bool menu_settings::deep_compare(nlohmann::json &current_settings, const nlohmann::json &default_settings, bool compare_value)
    {
        bool should_save = false;

        for (auto& e : default_settings.items())
        {
            const std::string& key = e.key();

            if (current_settings.count(key) == 0 || (compare_value && current_settings[key] != e.value()))
            {
                current_settings[key] = e.value();

                should_save = true;
            }
            else if (current_settings[key].is_object() && e.value().is_object())
            {
                if (deep_compare(current_settings[key], e.value(), compare_value))
                    should_save = true;
            }
            else if (!current_settings[key].is_object() && e.value().is_object())
            {
                current_settings[key] = e.value();

                should_save = true;
            }
            else if (current_settings[key].size() < e.value().size())
            {
                current_settings[key] = e.value();

                should_save = true;
            }
        }

        return should_save;
    }
    bool menu_settings::save()
    {
        std::string settings_file = std::getenv("appdata");
        settings_file += this->settings_location;

        std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
        nlohmann::json j = *this;
        file << j.dump(4);
        file.close();

        return true;
    }
    bool menu_settings::write_default_config()
    {
        std::string settings_file = std::getenv("appdata");
        settings_file += this->settings_location;

        std::ofstream file(settings_file, std::ios::out);
        nlohmann::json j = *this;
        file << j.dump(4);
        file.close();

        return true;
    }
}