#pragma once
#include "Language.h"
#include <memory>
#include <string>

void SerializeLanguage(const std::string& filepath, const Language& language);
std::unique_ptr<Language> UnserializeLanguage(const std::string& filepath);
