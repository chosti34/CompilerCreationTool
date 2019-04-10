#pragma once
#include "Language.h"
#include <memory>
#include <string>

// Записываем информацию о созданной пользователем модели языка в файл формата XML
void SerializeLanguage(const std::string& filepath, const Language& language);

// Создаем модель языка из информации записанной в файл формата XML
void UnserializeLanguage(const std::string& filepath, Language& language);
