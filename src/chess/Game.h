#pragma once

#include "Common.h"
#include "BoardRenderer.h"
#include "Engine.h"
#include "schism/Game/GameEvent/CallbackListener.h"
#include "schism/Game/GameEvent/CallbackBus.h"

namespace Chess
{
	// Will handle everything from networking to input processing
    class Game: public Schism::GameEvent::CallbackListener
	{
	public:
		explicit Game(BoardRenderer& renderer, Schism::GameEvent::CallbackBus& networkSendBus);

        void UndoMove();
		void ProcessInput(Schism::Event& e);
        void Update();
		void DrawBoard();
	private:
		struct State
		{
			bool pieceSelected{ false };
			Position selectedPosition{ 0, 0 };
			bool isWhite{ true };
			uint64_t gameId{ 0 };
            bool waitingForMove{ false };
            bool moveValidated{ true };
            uint16_t moveCount{ 0 };
		};

		[[nodiscard]] Position FindBoardPositionFromCoord(float x, float y) const
		{
			const auto offset = m_BoardRenderer.GetOffset();
			const auto pieceSize = m_BoardRenderer.GetPieceSize();

			const uint8_t gridX = (x - offset) / pieceSize;
			const uint8_t gridY = (y - offset) / pieceSize;

			return { gridX, gridY };
		}

		State m_State;
		Engine m_Engine;
		BoardRenderer& m_BoardRenderer;
		Schism::GameEvent::CallbackBus& m_NetworkSendBus;
		std::vector<Position> m_ValidMoves;
		float m_Offset{};
		float m_PieceSize{};
	};
}