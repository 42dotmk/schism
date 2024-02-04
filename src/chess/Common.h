#pragma once

#include <array>
#include <cstdint>
#include "msgpack/msgpack.hpp"

namespace Chess
{
    namespace Net
    {
        static constexpr size_t MAX_MESSAGE_BUFFER_LENGTH = 1024;
    }

	enum PieceType
	{
		PieceType_Pawn = 0,
		PieceType_Knight,
		PieceType_Bishop,
		PieceType_Queen,
		PieceType_Rook,
		PieceType_King,
		PieceType_Count,
		PieceType_Blank,
	};

	enum PieceColor
	{
		PieceColor_Black = 0,
		PieceColor_White
	};

	struct Piece
	{
		uint8_t type{ PieceType_Blank };
        uint8_t color{ PieceColor_White };

        template<typename T>
        void pack(T& p)
        {
            p(type, color);
        }
	};

	struct Position
	{
		uint8_t x;
		uint8_t y;

		bool operator == (const Position& rhs) const
		{
			return x == rhs.x && y == rhs.y;
		}

        template<typename T>
        void pack(T& p)
        {
            p(x, y);
        }
	}; 

	struct Move
	{
        Piece piece{ PieceType_Blank }; // Might be useless
		Position currentPosition{ 0, 0 };
		Position prevPosition{ 0, 0 };

        template<typename T>
        void pack(T& p)
        {
            p(piece, currentPosition, prevPosition);
        }
	};

	using Board = std::array<std::array<Piece, 8>, 8>;

	inline Position FlipBoardPosition(const Position& pos)
	{
		return { (uint8_t)(7 - pos.x), (uint8_t)(7 - pos.y) };
	}

	inline bool IsValidPiece(uint8_t type)
	{
		return (type != PieceType_Count) && (type != PieceType_Blank);
	}

	inline uint8_t InvertPieceColor(uint8_t color)
	{
		return color == PieceColor_White ? PieceColor_Black : PieceColor_White;
	}
}