#include "BoardRenderer.h"

#include <iostream>

#include "schism/Core/Events/MouseEvents.h"
#include "schism/Renderer/SpriteRenderer.h"

namespace Chess
{
	BoardRenderer::BoardRenderer()
		:
		m_Width{ 0 },
		m_Height{ 0 },
		m_PieceSize{ 0.f },
		m_Offset{ 0.f }
	{
	}

	void BoardRenderer::Init(Resources&& sprites, uint16_t width, uint16_t height,
		float pieceSize, float offsetPercentage)
	{
		m_Sprites = std::move(sprites);
		m_Width = width;
		m_Height = height;
		m_PieceSize = pieceSize;
		m_Offset = static_cast<float>(m_Width) * offsetPercentage;
	}

	void BoardRenderer::DrawValidMoves(const std::vector<Position>& validMoves)
	{
		static Schism::Components::Transform2D transform;

		for (auto& position : validMoves)
		{
			transform.position = {
					m_PieceSize * position.x + m_Offset,
					m_PieceSize * position.y + m_Offset,
                    0.f
			};
			transform.scale = { m_PieceSize, m_PieceSize };

			Schism::SpriteRenderer::Draw(transform, m_Sprites.validMove);
		}
	}

	void BoardRenderer::DrawBoard(const Board& board, bool flipped)
	{
		static Schism::Components::Transform2D transform;

		for (uint8_t x = 0; x < board.size(); x++)
		{
			for (uint8_t y = 0; y < board[0].size(); y++)
			{
				if (board[x][y].type == PieceType_Blank)
				{
					continue;
				}

				Schism::Components::Sprite* sprite = nullptr;

				if (board[x][y].color == PieceColor_White)
				{
					sprite = &m_Sprites.whiteSprites[board[x][y].type];
				}
				else
				{
					sprite = &m_Sprites.blackSprites[board[x][y].type];
				}

				int translatedX = x;
				int translatedY = y;

				if (flipped)
				{
					auto translated = FlipBoardPosition({ x, y });
					translatedX = translated.x;
					translatedY = translated.y;
				}

				transform.position = {
					m_PieceSize * translatedX + m_Offset,
					m_PieceSize * translatedY + m_Offset,
                    0.f,
				};
				transform.scale = { m_PieceSize, m_PieceSize };

				Schism::SpriteRenderer::Draw(transform, *sprite);
			}
		}

		transform.position = { 0.f, 0.f, 0.f };
		transform.scale = { m_Width, m_Height };

		Schism::SpriteRenderer::Draw(transform, m_Sprites.board);
	}
}
