#include "Engine.h"

#include "schism/System/Log.h"

namespace Chess
{
	Engine::Engine()
    :
    m_TurnWhite{ true }
	{
		Reset();
	}

	void Engine::Reset()
	{
		for (auto& line : m_Board)
		{
			line.fill({ });
		}

		for (int i = 0; i < 8; i++)
		{
			m_Board[i][1] = { PieceType_Pawn, PieceColor_Black };
			m_Board[i][6] = { PieceType_Pawn, PieceColor_White };
		}

		m_Board[0][0] = { PieceType_Rook, PieceColor_Black };
		m_Board[1][0] = { PieceType_Knight, PieceColor_Black };
		m_Board[2][0] = { PieceType_Bishop, PieceColor_Black };
		m_Board[3][0] = { PieceType_Queen, PieceColor_Black };
		m_Board[4][0] = { PieceType_King, PieceColor_Black };
		m_Board[5][0] = { PieceType_Bishop, PieceColor_Black };
		m_Board[6][0] = { PieceType_Knight, PieceColor_Black };
		m_Board[7][0] = { PieceType_Rook, PieceColor_Black };

		m_Board[0][7] = { PieceType_Rook, PieceColor_White };
		m_Board[1][7] = { PieceType_Knight, PieceColor_White };
		m_Board[2][7] = { PieceType_Bishop, PieceColor_White };
		m_Board[3][7] = { PieceType_Queen, PieceColor_White };
		m_Board[4][7] = { PieceType_King, PieceColor_White };
		m_Board[5][7] = { PieceType_Bishop, PieceColor_White };
		m_Board[6][7] = { PieceType_Knight, PieceColor_White };
		m_Board[7][7] = { PieceType_Rook, PieceColor_White };

		m_ValidMoves.clear();
		m_ValidMoves.reserve(m_Board.size() * 2);
        m_Moves.clear();
	}

    void Engine::UndoLastMove()
    {
        if (m_Moves.empty())
        {
            return;
        }
        auto& engineMove = m_Moves.back();

        auto& currentPosition = engineMove.move.currentPosition;
        auto& prevPosition = engineMove.move.prevPosition;

        m_Board[prevPosition.x][prevPosition.y] = m_Board[currentPosition.x][currentPosition.y];
        m_Board[currentPosition.x][currentPosition.y] = engineMove.takenPiece;
        m_TurnWhite = !m_TurnWhite;

        m_Moves.pop_back();
    }

	bool Engine::MakeMove(const Move& move)
	{
		const auto piece = m_Board[move.prevPosition.x][move.prevPosition.y];
		const auto nextPiece = m_Board[move.currentPosition.x][move.currentPosition.y];

        if (!(m_TurnWhite && piece.color == PieceColor_White)
            && !(!m_TurnWhite && piece.color == PieceColor_Black)
        )
        {
            return false;
        }
		// Check if we want to move to another piece by the same color
		if (move.piece.color == nextPiece.color && nextPiece.type != PieceType_Blank)
		{
			return false;
		}

		const auto& validMoves = GetValidMoves(move.prevPosition);
		if (validMoves.empty())
		{
			return false; // Figure what to do if no valid moves
		}

		for (auto& validMovePosition : validMoves)
		{
			if (validMovePosition == move.currentPosition)
			{
				// Implement takes piece
                auto prevPiece = m_Board[move.currentPosition.x][move.currentPosition.y];

                m_Board[move.prevPosition.x][move.prevPosition.y] = { PieceType_Blank };
				m_Board[move.currentPosition.x][move.currentPosition.y] = piece;
                m_TurnWhite = !m_TurnWhite;

                m_Moves.push_back({ move, prevPiece });
				return true;
			}
		}

		return false;
	}

	const std::vector<Position>& Engine::GetValidMoves(const Position& position)
	{
		m_ValidMoves.clear();

		if (position.x >= m_Board.size() || position.y >= m_Board.size())
		{
			return m_ValidMoves;
		}

		const auto& piece = m_Board[position.x][position.y];

		switch (piece.type)
		{
		case PieceType_Pawn:
		{
			return ValidMovesPawn(position);
		}
		case PieceType_Knight:
		{
			return ValidMovesKnight(position);
		}
		case PieceType_Bishop:
		{
			return ValidMovesBishop(position);
		}
		case PieceType_Queen:
		{
			return ValidMovesQueen(position);
		}
		case PieceType_King:
		{
			return ValidMovesKing(position);
		}
		case PieceType_Rook:
		{
			return ValidMovesRook(position);
		}
		default:
			return m_ValidMoves;
		}
	}

	inline bool Engine::CheckObstacle(std::vector<Position>& validMoves, const Piece& myPiece, const Position& position, bool canTake) const
	{
		if (position.y >= m_Board.size() ||
			position.x >= m_Board.size())
		{
			return false;
		}
		auto& piece = m_Board[position.x][position.y];

		if (canTake &&
			IsValidPiece(piece.type) &&
			piece.color == InvertPieceColor(myPiece.color))
		{
			validMoves.push_back(position);
			return false; // Stop if we find a valid piece to take
		}

		if (IsValidPiece(piece.type) && piece.color != InvertPieceColor(myPiece.color))
		{
			return false;
		}

		validMoves.push_back(position);

		return true;
	}


	void Engine::CheckObstacleVertically(std::vector<Position>& validMoves, const Position& position, uint8_t length, bool canTake, bool descending) const
	{
		const auto& myPiece = m_Board[position.x][position.y];
		
		for (uint8_t y = position.y + 1; y <= static_cast<uint8_t>(position.y + (length - 1)); y++)
		{
			if (!CheckObstacle(validMoves, myPiece, { position.x, y }, canTake))
			{
				break;
			}
		}

		if (descending)
		{
			const uint8_t yLimit = (length - 1) > position.y ? 0 : static_cast<uint8_t>(position.y - (length - 1));
			for (uint8_t y = position.y - 1; y >= yLimit; y--)
			{
				if (!CheckObstacle(validMoves, myPiece, { position.x, y }, canTake))
				{
					break;
				}
			}
		}
	}

	void Engine::CheckObstacleHorizontally(std::vector<Position>& validMoves, const Position& position, uint8_t length, bool canTake, bool descending) const
	
	{	// Make this into a single loop, can be optimized
		const auto& myPiece = m_Board[position.x][position.y];
		for (uint8_t x = position.x + 1; x <= static_cast<uint8_t>(position.x + (length - 1)); x++)
		{
			if (!CheckObstacle(validMoves, myPiece, { x, position.y }, canTake))
			{
				break;
			}
		}

		if (descending)
		{
			const uint8_t xLimit = (length - 1) > position.x ? 0 : static_cast<uint8_t>(position.x - (length - 1));
			for (uint8_t x = position.x - 1; x >= xLimit; x--)
			{
				if (!CheckObstacle(validMoves, myPiece, { x , position.y }, canTake))
				{
					break;
				}
			}
		}
	}

	void Engine::CheckObstacleDiagonally(std::vector<Position>& validMoves, const Position& position)
	{
		const auto& myPiece = m_Board[position.x][position.y];

		bool rightDiagonalAscending = true;
		bool rightDiagonalDescending = true;
		bool leftDiagonalAscending = true;
		bool leftDiagonalDescending = true;
		
		uint8_t count = 0;

		for (uint8_t i = 1; i < 16; i++)
		{
			if (rightDiagonalAscending)
			{
				const Position newPosition{
					static_cast<uint8_t>(position.x + i),
					static_cast<uint8_t>(position.y + i)
				};
				rightDiagonalAscending  = CheckObstacle(validMoves, myPiece, newPosition, true);
				count++;
			}
			if (leftDiagonalDescending)
			{
				const Position newPosition{
					static_cast<uint8_t>(position.x - i),
					static_cast<uint8_t>(position.y - i)
				};
				leftDiagonalDescending = CheckObstacle(validMoves, myPiece, newPosition, true);
				count++;
			}
			if (leftDiagonalAscending)
			{
				const Position newPosition{
					static_cast<uint8_t>(position.x - i),
					static_cast<uint8_t>(position.y + i)
				};
				leftDiagonalAscending = CheckObstacle(validMoves, myPiece, newPosition, true);
				count++;
			}
			if (rightDiagonalDescending)
			{
				const Position newPosition{
					static_cast<uint8_t>(position.x + i),
					static_cast<uint8_t>(position.y - i)
				};
				rightDiagonalDescending = CheckObstacle(validMoves, myPiece, newPosition, true);
				count++;
			}
			if (count == 0)
			{
				break;
			}
		}
	}

	const std::vector<Position>& Engine::ValidMovesPawn(const Position& position)
	{
		const auto& myPiece = m_Board[position.x][position.y];

		auto checkOpposingPiece = [&](const Position& newPosition)
		{
			if (newPosition.x >= m_Board.size() || newPosition.y >= m_Board.size())
			{
				return;
			}
			const auto& opposingPiece = m_Board[newPosition.x][newPosition.y];
			if (IsValidPiece(opposingPiece.type) &&
				myPiece.color == InvertPieceColor(opposingPiece.color))
			{
				m_ValidMoves.push_back(newPosition); // find a way to return the valid move instead of mutating the class object
			}
		};

		auto checkCanTake = [&](bool descending)
		{
			if (!descending)
			{
				const Position newPosLeft{
							static_cast<uint8_t>(position.x - 1),
							static_cast<uint8_t>(position.y + 1)
				};
				const Position newPosRight{
							static_cast<uint8_t>(position.x + 1),
							static_cast<uint8_t>(position.y + 1)
				};
				
				checkOpposingPiece(newPosLeft);
				checkOpposingPiece(newPosRight);
			}
			else
			{
				const Position newPosLeft{
					static_cast<uint8_t>(position.x - 1),
					static_cast<uint8_t>(position.y - 1)
				};
				const Position newPosRight{
							static_cast<uint8_t>(position.x + 1),
							static_cast<uint8_t>(position.y - 1)
				};

				checkOpposingPiece(newPosLeft);
				checkOpposingPiece(newPosRight);
			}
		};

		if (myPiece.color == PieceColor_White)
		{
			checkCanTake(true);
			if (position.y == 6)
			{
				CheckObstacleVertically(m_ValidMoves, position, 3, false, true);
			}
			else
			{
				// Need to make sure we setup queening
				if (position.y >= 1)
				{
					const Position newPos{ position.x, static_cast<uint8_t>(position.y - 1) };
					const auto& piece = m_Board[newPos.x][newPos.y];
					if (piece.type == PieceType_Blank)
					{
						m_ValidMoves.push_back(newPos);
					}
				}
			}
		}
		else 
		{
			checkCanTake(false);
			if (position.y == 1)
			{
				CheckObstacleVertically(m_ValidMoves, position, 3, false, false);
			}
			else
			{
				// Need to make sure we setup queening
				if (position.y <= 6)
				{
					const Position newPos{ position.x, static_cast<uint8_t>(position.y + 1) };
					const auto& piece = m_Board[newPos.x][newPos.y];
					if (piece.type == PieceType_Blank)
					{
						
						m_ValidMoves.push_back(newPos);
					}
				}
			}
		}

		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesKnight(const Position& position)
	{
		const auto& myPiece = m_Board[position.x][position.y];

		const Position topRight_1{
			static_cast<uint8_t>(position.x + 2),
			static_cast<uint8_t>(position.y - 1)
		};
		const Position topRight_2{
			static_cast<uint8_t>(position.x + 1),
			static_cast<uint8_t>(position.y - 2)
		};
		const Position bottRight_1 {
			static_cast<uint8_t>(position.x + 2),
			static_cast<uint8_t>(position.y + 1)
		};
		const Position bottRight_2{
			static_cast<uint8_t>(position.x + 1),
			static_cast<uint8_t>(position.y + 2)
		};
		const Position topLeft_1{
			static_cast<uint8_t>(position.x - 2),
			static_cast<uint8_t>(position.y - 1)
		};
		const Position topLeft_2{
			static_cast<uint8_t>(position.x - 1),
			static_cast<uint8_t>(position.y - 2)
		};
		const Position bottLeft_1{
			static_cast<uint8_t>(position.x - 2),
			static_cast<uint8_t>(position.y + 1)
		};
		const Position bottLeft_2{
			static_cast<uint8_t>(position.x - 1),
			static_cast<uint8_t>(position.y + 2)
		};

		CheckObstacle(m_ValidMoves, myPiece, topRight_1, true);
		CheckObstacle(m_ValidMoves, myPiece, bottRight_1, true);
		CheckObstacle(m_ValidMoves, myPiece, topRight_2, true);
		CheckObstacle(m_ValidMoves, myPiece, bottRight_2, true);
		CheckObstacle(m_ValidMoves, myPiece, topLeft_1, true);
		CheckObstacle(m_ValidMoves, myPiece, bottLeft_1, true);
		CheckObstacle(m_ValidMoves, myPiece, topLeft_2, true);
		CheckObstacle(m_ValidMoves, myPiece, bottLeft_2, true);

		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesBishop(const Position& position)
	{
		CheckObstacleDiagonally(m_ValidMoves, position);

		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesQueen(const Position& position)
	{
		CheckObstacleDiagonally(m_ValidMoves, position);
		CheckObstacleHorizontally(m_ValidMoves, position, 8, true, true);
		CheckObstacleVertically(m_ValidMoves, position, 8, true, true);

		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesKing(const Position& position)
	{
		return m_ValidMoves;
	}

	const std::vector<Position>& Engine::ValidMovesRook(const Position& position)
	{
		CheckObstacleHorizontally(m_ValidMoves, position, 8, true, true);
		CheckObstacleVertically(m_ValidMoves, position, 8, true, true);

		return m_ValidMoves;
	}

}
