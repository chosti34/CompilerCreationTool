#pragma once
#include "Language.h"
#include <memory>
#include <string>

// ���������� ���������� � ��������� ������������� ������ ����� � ���� ������� XML
void SerializeLanguage(const std::string& filepath, const Language& language);

// ������� ������ ����� �� ���������� ���������� � ���� ������� XML
void UnserializeLanguage(const std::string& filepath, Language& language);
