#pragma once
#include "Language.h"
#include <memory>
#include <string>

// ���������� ���������� � ��������� ������������� ������ ����� � ���� ������� XML
void SerializeLanguage(const std::string& filepath, const Language& language);

// ������� ������ ����� �� ���������� ���������� � ���� ������� XML
std::unique_ptr<Language> UnserializeLanguage(const std::string& filepath);
