#pragma once
#include <map>
#include <string>

using namespace std;

class LexemeType {
    enum Type {

        Identifier = 0,
        Number,

        Whitespace,
        LeftParen,
        RightParen,
        Letter,

        Val,
        Var,
        Add,
        If,
        Then,
        Else,
        Let,
        In,
        Function,
        Call,
        Eof,
        None,
    };
    Type type;
    explicit operator std::string() {
        switch(type) {
        }
    }
}

map<std::string, LexemeType>
    keywords{{"val", LexemeType::Val},   {"var", LexemeType::Var},
             {"Add", LexemeType::Add},   {"If", LexemeType::If},
             {"Then", LexemeType::Then}, {"Else", LexemeType::Else},
             {"Let", LexemeType::Let},   {"Function", LexemeType::Function},
             {"Call", LexemeType::Call}};
