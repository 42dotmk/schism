#pragma once

#include <vector>
#include <array>
#include <optional>
#include "Common.h"

namespace Chess
{
	class Engine
	{
	public:
		Engine();
		void Reset();
		// Todo: add undo move


		bool MakeMove(const Move& move);
        void UndoLastMove();

        [[nodiscard]] uint16_t GetMoveCount()
        {
            return m_Moves.size();
        }

        [[nodiscard]] const std::vector<Position>& GetValidMoves(const Position& position);
        [[nodiscard]] std::vector<Move> GetMoves();
        [[nodiscard]] const Board& GetBoardState() const
		{
			return m_Board; 
		}
    private:
        struct EngineMove
        {
            Move move;
            Piece takenPiece;
        };

		bool CheckObstacle(std::vector<Position>& validMoves, const Piece& myPiece, const Position& position, bool canTake) const;

		void CheckObstacleVertically(std::vector<Position>& validMoves, const Position& position, 
			uint8_t length, bool canTake, bool descending) const;

		void CheckObstacleHorizontally(std::vector<Position>& validMoves, const Position& position, 
			uint8_t length, bool canTake, bool descending) const;

		void CheckObstacleDiagonally(std::vector<Position>&  validMoves, const Position& position);

		[[nodiscard]] const std::vector<Position>& ValidMovesPawn(const Position& position);
		[[nodiscard]] const std::vector<Position>& ValidMovesKnight(const Position& position);
		[[nodiscard]] const std::vector<Position>& ValidMovesBishop(const Position& position);
		[[nodiscard]] const std::vector<Position>& ValidMovesQueen(const Position& position);
		[[nodiscard]] const std::vector<Position>& ValidMovesKing(const Position& position);
		[[nodiscard]] const std::vector<Position>& ValidMovesRook(const Position& position);

        std::vector<Position> m_ValidMoves;
        std::vector<EngineMove> m_Moves;
		Board m_Board;
        bool m_TurnWhite;
	};
}