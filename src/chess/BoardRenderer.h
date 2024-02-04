#pragma once

#include <optional>

#include "Common.h"
#include "Engine.h"
#include "schism/Components/Sprite.h"
#include "schism/Core/Events/Event.h"

namespace Chess
{
	class BoardRenderer
	{
	public:
		struct Resources
		{
			std::array<Schism::Components::Sprite, 9> whiteSprites;
			std::array<Schism::Components::Sprite, 9> blackSprites;
			Schism::Components::Sprite board;
			Schism::Components::Sprite validMove;
		};

		BoardRenderer();

		void Init(Resources&& sprites, uint16_t width, uint16_t height,
			float pieceSize, float offsetPercentage);

		void DrawValidMoves(const std::vector<Position>& validMoves);
		
		void DrawBoard(const Board& board, bool flipped);

		float GetOffset()
		{
			return m_Offset;
		}

		float GetPieceSize()
		{
			return m_PieceSize;
		}

	private:
		bool m_IsPieceSelected{ false };
		std::pair<uint8_t, uint8_t> m_SelectedPosition;
		Resources m_Sprites;
		uint16_t m_Width;
		uint16_t m_Height;
		float m_PieceSize;
		float m_Offset;
	};
}