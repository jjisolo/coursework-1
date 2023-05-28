#include "Program.hpp"

#include "../engine/Sprite.hpp"

#define isValidShader(shader) shader.has_value()

#define SPRITE_APPLY_NONE_EFFECTS         0
#define SPRITE_APPLY_HOVER_GOOD_EFFECT    1
#define SPRITE_APPLY_HOVER_BAD_EFFECT     2
#define SPRITE_APPLY_MOTION_BLUR_EFFECT   4

using namespace std;
using namespace glm;
using namespace Engine;
using namespace Engine::GFX;

static constexpr const glm::vec2  CARD_ASSET_RATIO                = {1.f,    1.5f};
static constexpr const glm::vec2  CARD_ASSET_SIZE_NON_NORMALIZED  = {150.0f, 150.0f};
static constexpr const glm::ivec2 CARD_ASSET_SIZE_NORMALIZED      = CARD_ASSET_SIZE_NON_NORMALIZED*CARD_ASSET_RATIO;
static constexpr const float      CARDS_ROW_OTHER_PLAYERS_Y_COORD = 0.02f;

static constexpr const vec3  CARD_INTENCITY_MASK_BAD   = {0.9, 0.8, 0.8};
static constexpr const vec3  CARD_INTENCITY_MASK_GOOD  = {0.9, 0.8, 0.8};

static constexpr auto GAME_DESCRIPTION =
R"(The goal of the game is to score the least number of points. In order to play, you need a 
deck of 36 cards and from 2 to 4 players.The first card dealer in the game is determined by
lot, in  the following games, players deal cards in turn. The deck is carefully shuffled, 
removed and 5 cards are dealt to each player.
	
The deliverer deals himself 4 cards, and the fifth card is put on the line.The remaining
deck is placed in the center of the table in a closed form.The player to the left of the
deliverer continues the game. He must put a card of the same suit or a card of the same 
value on the fifth card of the deliverer, if there is no such card, then the player takes
one card from the deck and if this card does not fit, then the player skips the turn. Some
cards have their own features in the game, namely: aces, queens, king of spades, nines,
sevens and sixes.
)";

static constexpr auto GAME_SPECIFICATION =
R"(The game is baked using GLFW(platform layer) OpenGL library, GLAD OpenGL functions
loader library. GLM for the mathematical calculations(such as vector manipulations,
matricies calculus etc.), and the SPDLOG for logging into the files, and finally,
immediate-mode UI library Dear ImGui.
)";

static constexpr auto GAME_CREDITS =
R"(This game was create as a subject of my university coursework.

The source code is publicly available under the MIT license, and
hosted on https://github.com/jjisolo/coursework-1.
)";

#include <iostream>

namespace Game
{
	Engine::Error GameProgram::onUserInitialize()
	{
        auto windowDimensions  = getWindowDimensions();

		// Load game background texture
  		ResourceManager::loadTexture("data/assets/background.jpg", false, "background");
		
        // Load the card reverse side textures.
	    ResourceManager::loadTexture("data/assets/card-back1.png", true, "card-back-blue");
        ResourceManager::loadTexture("data/assets/card-back2.png", true, "card-back-red");
        ResourceManager::loadTexture("data/assets/card-back3.png", true, "card-back-green");
	    ResourceManager::loadTexture("data/assets/card-back4.png", true, "card-back-yellow");	
		
		// Create and set sprite for the background.
		Sprite backgroundSprite;
		backgroundSprite .setSpriteSize({ windowDimensions.x, windowDimensions.y });
		backgroundSprite .bindTexture  ("background");

		m_mainMenuSprites .push_back(backgroundSprite);
        m_gameBoardGeneral.push_back(backgroundSprite);
		
		// Set up game board
		m_gameBoard.generateDeck();
		m_gameBoard.shuffleDeck();
	    m_gameBoard.assignCardsToThePlayers();

		// The entry point of the game is main menu.
		m_gameInfo.gameState = GameState::Main_Menu;
		
		return(Error::Ok);
	}

	Error GameProgram::onUserRelease()
	{

		return(Error::Ok);
	}

	Error GameProgram::onUserUpdate(GLfloat elapsedTime)
	{
	    auto windowDimensions = getWindowDimensions();
        
		if (m_escapeButtonPressed == true)
			m_showBoardMenuWindow = true;

        // Update mouse cursor position on the current frame.
        glfwGetCursorPos(getWindowPointer(), &m_mousePositionX, &m_mousePositionY);
		
		if (m_gameInfo.gameState == GameState::Game_Board)
		{
			bool waitAnimations = false;

			for (auto& animatedSprite : m_gameBoardCards)
				if (animatedSprite.getIsAnimated()) waitAnimations = true;

			// Arrange the cards sprites(split the cards array by the different
			// rendering ares by the card owner).
			if(!waitAnimations)
				updateGameBoardCardSprites(windowDimensions);

			// Try to get the shaders from the global shader/texture storage, if the get request fails
			// exit the program.
			auto shaderWrapperOrError = Engine::ResourceManager::getShader("spriteShader");
			if (shaderWrapperOrError.has_value())
			{
				(*shaderWrapperOrError).setFloat   ("elapsedTime", glfwGetTime() * 6);
				(*shaderWrapperOrError).setVector2f("screenResolution", (ivec2)Engine::Window::instance().getWindowDimensionsKHR());
			}

			// Animate individual sprite that is on gameboard group.
			for (auto& sprite : m_gameBoardGeneral) {
				sprite.render(m_SpriteRenderer);
			}

			// Animate and render individual card sprite.
			const auto spriteGroupSize = m_gameBoardCards.size();
			m_hoveredCardCopy.cardRank = CardRankLast;

			for (auto& sprite : m_gameBoardCards) {
				sprite.animate(m_elapsedTime);

				const bool applyBadEffect  = (sprite.getRenderFlag() & SPRITE_APPLY_HOVER_BAD_EFFECT)     == SPRITE_APPLY_HOVER_BAD_EFFECT;
				const bool applyGoodEffect = (sprite.getRenderFlag() & SPRITE_APPLY_HOVER_GOOD_EFFECT)    == SPRITE_APPLY_HOVER_GOOD_EFFECT;
				const bool applyBlurEffect = (sprite.getRenderFlag() & SPRITE_APPLY_MOTION_BLUR_EFFECT)   == SPRITE_APPLY_MOTION_BLUR_EFFECT;

				const auto   size     = sprite.getSpriteSize();
				const auto   color    = sprite.getSpriteColor();
				const auto   position = sprite.getSpritePosition();
				const auto   rotation = sprite.getSpriteRotation();
				
				// Apply shadows
				AnimatedSprite shadowSprite;
				shadowSprite.setSpriteSize    (size);
				shadowSprite.setMoveSpeed     (sprite.getMoveSpeed());
				shadowSprite.setSpriteRotation(sprite.getSpriteRotation());
				shadowSprite.bindTexture      (sprite.getBindedTexture());
				shadowSprite.setSpritePosition({ position.x + size.x / 14, position.y + size.y / 14 });

				(*shaderWrapperOrError).setInteger("applyShadowEffect", 1);
				shadowSprite.render(m_SpriteRenderer);
				(*shaderWrapperOrError).setInteger("applyShadowEffect", 0);

				if (applyBlurEffect) // Apply motion blur
				{
					(*shaderWrapperOrError).setInteger("applyGlowingEffect", 0, true);
					const float offsetX = 2.6f;
					const float offsetY = 2.6f;

					sprite.render(m_SpriteRenderer);

					(*shaderWrapperOrError).setInteger("applyMotionEffect", 1, true);
					for (auto x = 0; x < 3; ++x)
					{
						for (auto y = 0; y < 3; ++y)
						{
							AnimatedSprite spriteCopy;
							spriteCopy.setSpriteColor({ color.x, color.y, color.z });
							spriteCopy.setSpriteRotation(rotation);
							spriteCopy.setSpriteSize(size);
							spriteCopy.bindTexture(sprite.getBindedTexture());

							spriteCopy.setSpritePosition({ position.x + offsetX * x, position.y + offsetY * y });
							spriteCopy.render(m_SpriteRenderer);
							spriteCopy.setSpritePosition({ position.x - offsetX * x, position.y - offsetY * y });
							spriteCopy.render(m_SpriteRenderer);
						}
					}
					(*shaderWrapperOrError).setInteger("applyMotionEffect", 0, true);
				}
				else if (applyBadEffect || applyGoodEffect) // Apply glowing effect
				{
					(*shaderWrapperOrError).setVector3f("intencityMask", applyBadEffect ? CARD_INTENCITY_MASK_BAD : CARD_INTENCITY_MASK_GOOD);
					(*shaderWrapperOrError).setInteger("applyGlowingEffect", 1, true);

					sprite.render(m_SpriteRenderer);
				}
				else
				{
					sprite.render(m_SpriteRenderer);
				}
				
				(*shaderWrapperOrError).setInteger("applyGlowingEffect", 0, true);
				(*shaderWrapperOrError).setInteger("applyBlurEffect",    0, true);
			}
        
			renderGameBoardUI(windowDimensions);
        }
          
		if (m_gameInfo.gameState == GameState::Main_Menu)
		{
          for (auto& sprite : m_mainMenuSprites)
            sprite.render(m_SpriteRenderer);

          renderMainMenuUI(windowDimensions);
		}
		
		return(Error::Ok);
	}

    void GameProgram::updateGameBoardCardSprites(ivec2& windowDimensions)
    { 
	  vector<Card>& cards = m_gameBoard.getCards();
	  
	  // Create a card group based on the card owner.
	  auto search = [](vector<Card>& cards, CardOwner owner) -> vector<Card> {
		vector<Card> ownerGroup;
		
		// Iterate through each card and if the card owner matches with the
		// requested owner add it to the result group(that contains only
		// owners that user specified).
		for(auto& card: cards)
		  if(card.cardOwner == owner)
			ownerGroup.push_back(card);
	    
		return(move(ownerGroup));
	  };	 
	  
      // Clear the previous sprites.
      m_gameBoardCards.clear();

	  // Create owner groups for each CardOwner
	  vector<Card> ownerHeap  = search(cards, CARD_OWNER_DECK);
	  vector<Card> ownerBoard = search(cards, CARD_OWNER_BOARD);
	  
	  // Render the cards that are in the heap.
	  // Render the cards that are in the deck.
	  auto deckOwnerGroup     = search(cards, CARD_OWNER_DECK);
	  auto deckOwnerGroupSize = deckOwnerGroup.size();

	  // The position of the deck is in middle right corner of
	  // the screen.
	  vec2 deckPosition;
	  deckPosition.x = windowDimensions.x * 0.85f;
	  deckPosition.y = windowDimensions.y * 0.40f;

	  // The idea is to render the deck like it is not aligned
	  // perfectly(not each card stacked on each other). Like	  
	  // there are some cards went out of the deck.
	  if (deckOwnerGroupSize >= 3)
	  {
		  AnimatedSprite veryTintedCard;
		  veryTintedCard.setSpritePosition(deckPosition);
		  veryTintedCard.setSpriteSize(CARD_ASSET_SIZE_NORMALIZED);
		  veryTintedCard.setSpriteRotation(25.0f);
		  veryTintedCard.bindTexture(deckOwnerGroup[2].textureHandleBack);
		  veryTintedCard.move(deckPosition); // No move, the sprite is static.
		  veryTintedCard.setRenderFlag(SPRITE_APPLY_NONE_EFFECTS);

		  m_gameBoardCards.push_back(veryTintedCard);
	  }

	  if (deckOwnerGroupSize >= 2)
	  {
		  AnimatedSprite slightlyTintedCard;
		  slightlyTintedCard.setSpritePosition(deckPosition);
		  slightlyTintedCard.setSpriteSize(CARD_ASSET_SIZE_NORMALIZED);
		  slightlyTintedCard.setSpriteRotation(15.0f);
		  slightlyTintedCard.bindTexture(deckOwnerGroup[1].textureHandleBack);
		  slightlyTintedCard.move(deckPosition); // No move, the sprite is static.			
		  slightlyTintedCard.setRenderFlag(SPRITE_APPLY_NONE_EFFECTS);

		  m_gameBoardCards.push_back(slightlyTintedCard);
	  }

	  if (deckOwnerGroupSize >= 1)
	  {
		  AnimatedSprite regularCard;
		  regularCard.setSpritePosition(deckPosition);
		  regularCard.setSpriteSize(CARD_ASSET_SIZE_NORMALIZED);
		  regularCard.setSpriteRotation(3.0f);
		  regularCard.bindTexture(deckOwnerGroup[0].textureHandleBack);
		  regularCard.move(deckPosition); // No move, the sprite is static.			
		  regularCard.setRenderFlag(SPRITE_APPLY_NONE_EFFECTS);

		  m_gameBoardCards.push_back(regularCard);
	  }


	  // Render the cards that are on the board.
	  auto renderPlayerCards = [&](vec2 renderAreaStart, vec2 renderAreaEnd, CardOwner cardOwner)
	  {
		auto ownerGroup     = search(cards, cardOwner);
	    auto ownerGroupSize = ownerGroup.size();
		
		// The offset between the cards so they can fit in the render area
		const float cardOffset = (renderAreaEnd.x - renderAreaStart.x) / ownerGroupSize;

		// Generate the sprite for each card in the card group.
	    for(size_t cardIndex=0; cardIndex < ownerGroupSize; ++cardIndex)
		{
		  AnimatedSprite playerCard;
	      playerCard.setSpriteSize    (CARD_ASSET_SIZE_NORMALIZED);
		  playerCard.setMoveSpeed     ({450.0f, 450.0f});
		  playerCard.setSpritePosition({renderAreaStart.x + cardOffset * cardIndex, renderAreaStart.y });
		  //playerCard.setSpritePosition(deckPosition);
		  playerCard.move({ renderAreaStart.x + cardOffset * cardIndex, renderAreaStart.y });

		  if((cardOwner == CARD_OWNER_PLAYER1 || cardOwner == CARD_OWNER_HEAP) || m_openCardsMode)
			playerCard.bindTexture(ownerGroup[cardIndex].textureHandleMain);
		  else
			playerCard.bindTexture(ownerGroup[cardIndex].textureHandleBack);

		  if (playerCard.getIsAnimated())
			  playerCard.setRenderFlag(playerCard.getRenderFlag() | SPRITE_APPLY_MOTION_BLUR_EFFECT);

		  if (cardOwner == CARD_OWNER_PLAYER1)
		  {
			  if (playerCard.isHovered({ m_mousePositionX, m_mousePositionY }, { 1.0f, 1.0f }))
			  {
				  playerCard.setRenderFlag(SPRITE_APPLY_HOVER_BAD_EFFECT);
					
				  if (m_mouseButtonPressed)
				  {
					  playerCard.move({ 100.0f, 100.0f });
				  }
			  }  
			  else
			  {
				  playerCard.setRenderFlag(SPRITE_APPLY_NONE_EFFECTS);
			  }
		  }
		  else
		  {
			  playerCard.setRenderFlag(SPRITE_APPLY_NONE_EFFECTS);
		  }
			  		  
		  m_gameBoardCards   .push_back(playerCard);
          m_gameBoardCardsRef.push_back(ownerGroup[cardIndex]);
		}
	  };

	  vec2 renderAreaStart;
      vec2 renderAreaEnd;
      
	  // Player 1
	  renderAreaStart.x = windowDimensions.x * 0.30f;
	  renderAreaStart.y = windowDimensions.y * 0.76;
	  renderAreaEnd.x   = windowDimensions.x * 0.70f;
	  renderAreaEnd.y   = renderAreaStart.y;
	  renderPlayerCards(renderAreaStart, renderAreaEnd, CARD_OWNER_PLAYER1);
	  
	  // Player 2
	  renderAreaStart.x = windowDimensions.x * 0.05f;
	  renderAreaStart.y = windowDimensions.y * CARDS_ROW_OTHER_PLAYERS_Y_COORD;
	  renderAreaEnd.x   = windowDimensions.x * 0.25f;
	  renderAreaEnd.y   = renderAreaStart.y;
	  renderPlayerCards(renderAreaStart, renderAreaEnd, CARD_OWNER_PLAYER2);

	  // Player 3
	  renderAreaStart.x = windowDimensions.x * 0.38f;
	  renderAreaStart.y = windowDimensions.y * CARDS_ROW_OTHER_PLAYERS_Y_COORD;
	  renderAreaEnd.x   = windowDimensions.x * 0.60f;
	  renderAreaEnd.y   = renderAreaStart.y;
	  renderPlayerCards(renderAreaStart, renderAreaEnd, CARD_OWNER_PLAYER3);

	  // Player 4
	  renderAreaStart.x = windowDimensions.x * 0.75f;
	  renderAreaStart.y = windowDimensions.y * CARDS_ROW_OTHER_PLAYERS_Y_COORD;
	  renderAreaEnd.x   = windowDimensions.x * 0.95f;
	  renderAreaEnd.y   = renderAreaStart.y;
	  renderPlayerCards(renderAreaStart, renderAreaEnd, CARD_OWNER_PLAYER4);
    }

    void GameProgram::renderPlayerStatUI(CardOwner owner)
    {
      ImGuiWindowFlags windowFlags = 0;
      //windowFlags |= ImGuiWindowFlags_NoMove;
      //windowFlags |= ImGuiWindowFlags_NoResize;
      windowFlags |= ImGuiWindowFlags_NoCollapse;

      if(!ImGui::Begin("Player Statistics", NULL, windowFlags))
      {
        ImGui::End();
      }
      else
      {
        ImGui::End();
      }
    }

    void GameProgram::renderGameBoardUI(ivec2& windowDimensions)
    {
	  ImguiCreateNewFrameKHR();
	  ImGui::NewFrame(); 
	  
	  ImGuiWindowFlags window_flags = 0;
	  window_flags |= ImGuiWindowFlags_NoResize;
	  window_flags |= ImGuiWindowFlags_NoCollapse;
	  window_flags |= ImGuiWindowFlags_NoMove;

	  if (m_showBoardMenuWindow)
	  {
		  if (!ImGui::Begin("101 Menu", &m_showBoardMenuWindow, window_flags))
		  {
			  ImGui::End();
		  }
		  else
		  {
			  if (ImGui::Button("\t\t\tContinue\t\t\t"))
			  {
				  m_showSettingsWindow = !m_showSettingsWindow;
			  } ImGui::SameLine();

			  
			  if (ImGui::Button("\t\t\tSave\t\t\t"))
			  {
				  
			  } ImGui::SameLine();

			  if (ImGui::Button("\t\t\tSettings\t\t\t"))
			  {
				  m_showSettingsWindow = true;
			  } ImGui::SameLine();

			  if (ImGui::Button("\t\t\tQuit\t\t\t"))
			  {
				  m_showQuitApproveWindow = !m_showQuitApproveWindow;
			  } ImGui::SameLine();

			  ImGui::End();
		  }
	  }

	  renderSettingsUI();
	  renderQuitApproveUI();

	  ImGui::EndFrame();
	  ImGui::Render();
    }

	void GameProgram::renderQuitApproveUI()
	{
		if (m_showQuitApproveWindow)
		{
			auto windowDimensions = getWindowDimensions();

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;

			ImGui::SetNextWindowPos(ImVec2(windowDimensions.x / 2.5, windowDimensions.y / 3.5), ImGuiCond_FirstUseEver);

			if (!ImGui::Begin("Do you really want to quit?", &m_showQuitApproveWindow, window_flags))
			{
				ImGui::End();
			}
			else
			{
				if (ImGui::Button("\t\t\tYes\t\t\t"))
				{
					glfwSetWindowShouldClose(getWindowPointer(), true);
				}

				ImGui::SameLine();

				if (ImGui::Button("\t\tNo\t\t\t"))
				{
					m_showQuitApproveWindow = false;
				}

				ImGui::End();
			}
		}
	}

	void GameProgram::renderSettingsUI()
	{
		if (m_showSettingsWindow)
		{
			auto windowDimensions = getWindowDimensions();

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoCollapse;

			ImGui::SetNextWindowPos(ImVec2(windowDimensions.x / 4, windowDimensions.y / 4), ImGuiCond_FirstUseEver);

			if (!ImGui::Begin("Settings", &m_showSettingsWindow, window_flags))
			{
				ImGui::End();
			}
			else
			{
				if (ImGui::CollapsingHeader("Game Rules"))
				{
					ImGui::Text(GAME_DESCRIPTION);
				}

				if (ImGui::CollapsingHeader("Game specification"))
				{
					ImGui::Text(GAME_SPECIFICATION);
				}

				if (ImGui::CollapsingHeader("ImGui User Manual"))
				{
					ImGui::ShowUserGuide();
				}

				if (ImGui::CollapsingHeader("Author Credits"))
				{
					ImGui::Text(GAME_CREDITS);
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Developer Tools"))
				{
					ImGui::MenuItem("Show Debug Window", NULL, &m_showDebugWindow);
					ImGui::MenuItem("Show Enemy Card Faces", NULL, &m_openCardsMode);

					ImGui::EndMenu();
				}

				ImGui::End();
			}
		}
	}

    void GameProgram::renderMainMenuUI(ivec2& windowDimensions)
    {
			ImguiCreateNewFrameKHR();
			ImGui::NewFrame(); 

			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;

			ImGui::SetNextWindowPos(
				ImVec2(windowDimensions.x/3-100, windowDimensions.y/2-100),
				ImGuiCond_FirstUseEver);
			ImGui::Begin("Main Menu", NULL, window_flags);
			
			if (ImGui::Button("\t\t\tNew Game\t\t\t"))
			  m_gameInfo.gameState = GameState::Game_Board; 
			
			ImGui::SameLine();

			if (ImGui::Button("\t\t\tContinue\t\t\t"))
				(void)0;

			ImGui::SameLine();

			if (ImGui::Button("\t\t\tSettings\t\t\t"))
				m_showSettingsWindow = !m_showSettingsWindow;

			ImGui::SameLine();

			if (ImGui::Button("\t\t\tQuit\t\t\t"))
				m_showQuitApproveWindow = !m_showQuitApproveWindow;
			
			ImGui::Separator();
			ImGui::Text(GAME_DESCRIPTION);
			ImGui::End();
			
			renderQuitApproveUI();
			renderSettingsUI();
            
			ImGui::EndFrame();
			ImGui::Render();
    }
}
