#pragma once

#include <plog/Log.h>

#include <nlohmann/json.hpp>

namespace msh::utils {

namespace JsonConfig {
using json = nlohmann::json;

template <typename T>
inline T getSafe(const json& j, const std::string& key, const T& default_value);

template <typename T>
inline T getSafe(const json& j, const std::string& key);

template <typename T>
T getSafe(const json& j, const std::string& key, const T& default_value) {
    if (!j.contains(key)) {
        if constexpr (std::is_enum_v<T>) {
            PLOG_WARNING << "j[\"" << key << "\"] is not available & default value set: \""
                         << static_cast<std::underlying_type_t<T>>(default_value) << "\"";
        } else {
            PLOG_WARNING << "j[\"" << key << "\"] is not available & default value set: \""
                         << default_value << "\"";
        }
        return default_value;
    }

    const auto& jvalue = j[key];

    if (jvalue.is_null()) {
        if constexpr (std::is_enum_v<T>) {
            PLOG_WARNING << "j[\"" << key << "\"] is null & default value set: \""
                         << static_cast<std::underlying_type_t<T>>(default_value) << "\"";
            return default_value;
        } else {
            PLOG_WARNING << "j[\"" << key << "\"] is null & default value set: \"" << default_value
                         << "\"";
            return default_value;
        }
    }

    if constexpr (std::is_enum_v<T>) {
        if (jvalue.is_string()) {
            try {
                return jvalue.get<T>();
            } catch (const std::exception& e) {
                PLOG_WARNING << "j[\"" << key.c_str() << "\"] enum conversion failed: " << e.what()
                             << " | value: \"" << jvalue.dump() << "\"";
            }
            return default_value;
        }
    } else if constexpr (std::is_same_v<T, bool>) {
        if (jvalue.is_boolean()) {
            return jvalue.get<T>();
        }
    } else if constexpr (std::is_integral_v<T>) {
        if (jvalue.is_number_integer()) {
            auto value = jvalue.get<int64_t>();
            if (value >= static_cast<int64_t>(std::numeric_limits<T>::min()) &&
                value <= static_cast<int64_t>(std::numeric_limits<T>::max())) {
                return static_cast<T>(value);
            } else {
                return default_value;
            }
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        if (jvalue.is_number_float()) {
            return jvalue.get<T>();
        }
    } else if constexpr (std::is_same_v<T, std::string>) {
        if (jvalue.is_string()) {
            return jvalue.get<T>();
        }
    }

    PLOG_WARNING << "j[\"" << key << "\"] has invalid type: \"" << jvalue.type_name()
                 << "\" & value: \"" << jvalue.dump() << "\"";
    return default_value;
}

template <typename T>
T getSafe(const json& j, const std::string& key) {
    if (!j.contains(key)) {
        if constexpr (std::is_enum_v<T>) {
            PLOG_WARNING << "j[\"" << key << "\"] is not available & default value set: \""
                         << static_cast<std::underlying_type_t<T>>(T{}) << "\"";
        } else {
            PLOG_WARNING << "j[\"" << key << "\"] is not available & default value set: \"" << T{}
                         << "\"";
        }
        return T{};
    }

    const auto& jvalue = j[key];

    if (jvalue.is_null()) {
        if constexpr (std::is_enum_v<T>) {
            PLOG_WARNING << "j[\"" << key << "\"] is null & default value set: \""
                         << static_cast<std::underlying_type_t<T>>(T{}) << "\"";
        } else {
            PLOG_WARNING << "j[\"" << key << "\"] is null & default value set: \"" << T{} << "\"";
        }
        return T{};
    }

    if constexpr (std::is_enum_v<T>) {
        if (jvalue.is_string()) {
            try {
                return jvalue.get<T>();
            } catch (const std::exception& e) {
                PLOG_WARNING << "j[\"" << key.c_str() << "\"] enum conversion failed: " << e.what()
                             << " | value: \"" << jvalue.dump() << "\"";
            }
            return T{};
        }
    } else if constexpr (std::is_same_v<T, bool>) {
        if (jvalue.is_boolean()) {
            return jvalue.get<T>();
        }
    } else if constexpr (std::is_integral_v<T>) {
        if (jvalue.is_number_integer()) {
            auto value = jvalue.get<int64_t>();
            if (value >= static_cast<int64_t>(std::numeric_limits<T>::min()) &&
                value <= static_cast<int64_t>(std::numeric_limits<T>::max())) {
                return static_cast<T>(value);
            } else {
                return T{};
            }
        }
    } else if constexpr (std::is_floating_point_v<T>) {
        if (jvalue.is_number_float()) {
            return jvalue.get<T>();
        }
    } else if constexpr (std::is_same_v<T, std::string>) {
        if (jvalue.is_string()) {
            return jvalue.get<T>();
        }
    }

    PLOG_WARNING << "j[\"" << key << "\"] has invalid type: \"" << jvalue.type_name()
                 << "\" & value: \"" << jvalue.dump() << "\"";
    return T{};
}
}  // namespace JsonConfig

}  // namespace msh::utils