#pragma once

#include "In.h"
#include "LT.h"
#include "IT.h"

namespace Lexer {
    void Analyze(In::IN& in, LT::LexTable& lt, IT::IdTable& it);
}