#include "Application.h"





//args[0]: level(terrain) type (0:city , 1:forest)
//args[1]: mode (0: level mode, 1: endless mode)
void Application::game(vector<int> args) {
	ifstream in("save.txt");
	SDL_Event e;
	SDL_Rect rect;
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	//load the resource
	loadResource();
	//load collision effect
	bool isPlaying = true;
	vector<LTexture*> collisionFrames = resourceManager.GetTexture(ResourceType::Explosion);
	AnimatingObject* collisionEffect = nullptr;
	int currCollisionFrame = 0;
	int lastPlayTime = 0;
	//Character player(gRenderer, resourceManager.GetTexture(ResourceType::Character), 4, 10, 300, 610, 32, 32, 200);

	//Character player(gRenderer, resourceManager.GetTexture(ResourceType::Character), in);
	Character* player;
	LevelGenerator* levelGenerator;
	if (args.size() == 3) {
		in >> isPlaying >> currCollisionFrame >> lastPlayTime >> args[0] >> args[1];
		player = new Character(gRenderer, resourceManager.GetTexture(ResourceType::Character), in);
		if (args[1] == 0) {
			levelGenerator = new RandomLevelGenerator(player, in);
		}
		else {
			levelGenerator = new EndlessLevelGenerator(player, in);
		}
	}
	else {
		player = new Character(gRenderer, resourceManager.GetTexture(ResourceType::Character), 4, 10, 300, 610, 32, 32, 200);
		if (args[1] == 0) {
			levelGenerator = new RandomLevelGenerator(0, 100, player, args[0]);
		}
		else {
			levelGenerator = new EndlessLevelGenerator(0, 100, player, args[0]);
		}
	}


	int score;
	Uint32 startTime = SDL_GetTicks();
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	bool paused = false;
	SDL_Rect pauseButtonDestRect = { 10,10,50,50 };
	SDL_Rect continueButtonDestRect = { SCREEN_WIDTH / 2 - 200 / 2,150,200,100 };
	SDL_Rect saveButtonDestRect = { SCREEN_WIDTH / 2 - 200 / 2,275,200,100 };
	SDL_Rect quitButtonDestRect = { SCREEN_WIDTH / 2 - 200 / 2,400,200,100 };
	Button* pauseButton = new Button("../../../resources/GameScreen/Button/pauseNormal.png", "../../../resources/GameScreen/Button/pauseHover.png", pauseButtonDestRect, nullptr);
	Button* continueButton = new Button("../../../resources/GameScreen/Button/continueNormal.png", "../../../resources/GameScreen/Button/continueHover.png", pauseButtonDestRect, nullptr);
	Button* continueButtonB = new Button("../../../resources/GameScreen/Button/continueNormalB.png", "../../../resources/GameScreen/Button/continueHoverB.png", continueButtonDestRect, nullptr);
	Button* saveButton = new Button("../../../resources/GameScreen/Button/saveNormal.png", "../../../resources/GameScreen/Button/saveHover.png", saveButtonDestRect, nullptr);
	Button* quitButton = new Button("../../../resources/GameScreen/Button/quitNormal.png", "../../../resources/GameScreen/Button/quitHover.png", quitButtonDestRect, &levelScreen);


	//if (args.size() == 3) {
		//in >> isPlaying >> currCollisionFrame >> lastPlayTime >> args[0] >> args[1];
		//player = Character(gRenderer, resourceManager.GetTexture(ResourceType::Character), in);

	//}
	in.close();
	bool quit = false;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		if (isPlaying && !paused)
			isPlaying &= levelGenerator->Update();
		levelGenerator->Draw();
		if (paused) {
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
			SDL_RenderFillRect(gRenderer, &SCREEN);
			continueButtonB->Update();
			continueButtonB->Draw();
			saveButton->Update();
			saveButton->Draw();
			quitButton->Update(&quit);
			quitButton->Draw();
		}
		if (!paused) {
			pauseButton->Update();
			pauseButton->Draw();
		}
		else {
			continueButton->Update();
			continueButton->Draw();
		}
		if (!isPlaying) {
			if (!collisionEffect) {
				SDL_Rect characterRect = player->getBoundingRect();
				collisionEffect = new NormalVehicle(gRenderer, collisionFrames, 12, 5, characterRect.x - 25, characterRect.y - 25, -1, -1, 0);
			}
			else if (currCollisionFrame >= 60) {
				quit = true;
				functionStack.push(VoidFunction(levelScreen));
			}
			collisionEffect->Update();
			collisionEffect->Draw();
			currCollisionFrame++;
		}
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			SDL_Point p;
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_MOUSEBUTTONDOWN:
				p = { e.button.x,e.button.y };
				if (SDL_PointInRect(&p, &pauseButtonDestRect))
					paused = !paused;
				if (paused && SDL_PointInRect(&p, &continueButtonDestRect)) {
					paused = false;
				}
				if (paused && SDL_PointInRect(&p, &saveButtonDestRect)) {
					ofstream out("save.txt");
					out << isPlaying << " " << currCollisionFrame << " " << (SDL_GetTicks() - startTime) + lastPlayTime << " " << args[0] << " " << args[1] << endl;
					player->ToFile(out);
					levelGenerator->ToFile(out);
					out.close();
				}
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_P) {
					paused = !paused;
				}
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	score = levelGenerator->getScore();
	Uint32 timeInMili = (SDL_GetTicks() - startTime) + lastPlayTime;
	int totalSeconds = timeInMili / 1000;
	string minutes = to_string(totalSeconds / 60);
	string seconds = to_string(totalSeconds % 60);
	if (minutes.size() == 1) minutes = "0" + minutes;
	if (seconds.size() == 1) seconds = "0" + seconds;
	string timeFormatted = minutes + ":" + seconds;
	saveScore(args[1] == 0 ? "L" : "E", score, timeFormatted, args[0] == 0 ? "CITY" : "FOREST");

	delete collisionEffect;
	delete pauseButton;
	delete continueButton;
	delete continueButtonB;
	delete quitButton;
	delete saveButton;
	delete player;
	delete levelGenerator;

}

void Application::leaderboardScreen() {
	const SDL_Rect bgRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	LTexture* bg = new LTexture(gRenderer, "../../../resources/LeaderboardScreen/Background/bg.png");

	const SDL_Rect homeMenuRect = { 68,32,-1,-1 };
	Button* homeMenu = new Button("../../../resources/LeaderboardScreen/Button/homeMenuNormal.png", "../../../resources/LeaderboardScreen/Button/homeMenuHover.png", homeMenuRect, &mainScreen);

	const SDL_Rect playMenuRect = { 172,32,-1,-1 };
	Button* playMenu = new Button("../../../resources/LeaderboardScreen/Button/playMenuNormal.png", "../../../resources/LeaderboardScreen/Button/playMenuHover.png", playMenuRect, &levelScreen);

	const SDL_Rect settingMenuRect = { 281,32,-1,-1 };
	Button* settingMenu = new Button("../../../resources/LeaderboardScreen/Button/settingMenuNormal.png", "../../../resources/LeaderboardScreen/Button/settingMenuHover.png", settingMenuRect, &settingScreen);

	const SDL_Rect scoreMenuRect = { 422,32,-1,-1 };
	Button* scoreMenu = new Button("../../../resources/LeaderboardScreen/Button/scoreMenuHover.png", "../../../resources/LeaderboardScreen/Button/scoreMenuHover.png", scoreMenuRect, nullptr);

	const SDL_Rect creditMenuRect = { 534,32,-1,-1 };
	Button* creditMenu = new Button("../../../resources/LeaderboardScreen/Button/creditMenuNormal.png", "../../../resources/LeaderboardScreen/Button/creditMenuHover.png", creditMenuRect, &creditScreen);

	TTF_Font* font = TTF_OpenFont("../../../resources/Font/SuperLegendBoy.ttf", 16);
	TTF_Font* fontScaled = TTF_OpenFont("../../../resources/Font/SuperLegendBoy.ttf", 24);
	bool quit = false;
	SDL_Event e;

	ifstream in("score.txt");
	assert(in.is_open(), "The file score.txt cannot be opened");
	vector<tuple<string, string, int, string, string>> scores;
	string time, mode, terrainType;
	int score;
	string totalTime;
	while (in >> time >> mode >> score >> totalTime >> terrainType) {
		scores.push_back(make_tuple(time, mode, score, totalTime, terrainType));
	}
	in.close();
	sort(scores.begin(), scores.end(), cmp);

	const int scaledTopX = 206;
	const int scaledTopY = 135;
	const int totalRank = 6;
	const int startX = 255;
	const int startY = 290;
	const int gapX[5] = { 213,125,120,144 };
	const int gapY = 50;
	int lim = scores.size() <= totalRank ? scores.size() : totalRank;
	LTexture* text = new LTexture(gRenderer);
	cout << scores.size() << endl;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		bg->render(bgRect.x, bgRect.y, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT);

		homeMenu->Update(&quit);
		homeMenu->Draw();

		playMenu->Update(&quit);
		playMenu->Draw();

		settingMenu->Update(&quit);
		settingMenu->Draw();

		scoreMenu->Update();
		scoreMenu->Draw();

		creditMenu->Update(&quit);
		creditMenu->Draw();
		for (int rank = 0; rank < lim; rank++) {
			tie(time, mode, score, totalTime, terrainType) = scores[rank];

			text->loadFromRenderedText(time, WHITE, font);
			text->render(startX, startY + gapY * rank, NULL, -1, -1);

			text->loadFromRenderedText(mode, WHITE, font);
			text->render(startX + gapX[0], startY + gapY * rank, NULL, -1, -1);

			text->loadFromRenderedText(to_string(score), WHITE, font);
			text->render(startX + gapX[0] + gapX[1], startY + gapY * rank, NULL, -1, -1);

			text->loadFromRenderedText(totalTime, WHITE, font);
			text->render(startX + gapX[0] + gapX[1] + gapX[2], startY + gapY * rank, NULL, -1, -1);

			text->loadFromRenderedText(terrainType, WHITE, font);
			text->render(startX + gapX[0] + gapX[1] + gapX[2] + gapX[3], startY + gapY * rank, NULL, -1, -1);
		}

		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	delete bg;
	delete homeMenu;
	delete playMenu;
	delete settingMenu;
	delete scoreMenu;
	delete creditMenu;
	delete text;
	TTF_CloseFont(font);
	TTF_CloseFont(fontScaled);

}

void Application::settingScreen() {
	const SDL_Rect bgRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	LTexture* bg = new LTexture(gRenderer, "../../../resources/SettingScreen/Background/bg.png");

	const SDL_Rect homeMenuRect = { 68,32,-1,-1 };
	Button* homeMenu = new Button("../../../resources/SettingScreen/Button/homeMenuNormal.png", "../../../resources/SettingScreen/Button/homeMenuHover.png", homeMenuRect, &mainScreen);

	const SDL_Rect playMenuRect = { 172,32,-1,-1 };
	Button* playMenu = new Button("../../../resources/SettingScreen/Button/playMenuNormal.png", "../../../resources/SettingScreen/Button/playMenuHover.png", playMenuRect, &levelScreen);

	const SDL_Rect settingMenuRect = { 281,32,-1,-1 };
	Button* settingMenu = new Button("../../../resources/SettingScreen/Button/settingMenuHover.png", "../../../resources/SettingScreen/Button/settingMenuHover.png", settingMenuRect, nullptr);

	const SDL_Rect scoreMenuRect = { 422,32,-1,-1 };
	Button* scoreMenu = new Button("../../../resources/SettingScreen/Button/scoreMenuNormal.png", "../../../resources/SettingScreen/Button/scoreMenuHover.png", scoreMenuRect, &leaderboardScreen);

	const SDL_Rect creditMenuRect = { 534,32,-1,-1 };
	Button* creditMenu = new Button("../../../resources/SettingScreen/Button/creditMenuNormal.png", "../../../resources/SettingScreen/Button/creditMenuHover.png", creditMenuRect, &creditScreen);

	GlobalSetting temporarySetting = globalSetting;

	TTF_Font* font = TTF_OpenFont("../../../resources/Font/ARCADECLASSIC.ttf", 28);

	LTexture* upKeyText = new LTexture(gRenderer);
	upKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.up), WHITE, font);
	SDL_Rect upTextRect = { 574,316,upKeyText->getWidth(),upKeyText->getHeight() };

	LTexture* downKeyText = new LTexture(gRenderer);
	downKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.down), WHITE, font);
	SDL_Rect downTextRect = { 574,371,downKeyText->getWidth(),downKeyText->getHeight() };

	LTexture* leftKeyText = new LTexture(gRenderer);
	leftKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.left), WHITE, font);
	SDL_Rect leftTextRect = { 574,426,leftKeyText->getWidth(),leftKeyText->getHeight() };

	LTexture* rightKeyText = new LTexture(gRenderer);
	rightKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.right), WHITE, font);
	SDL_Rect rightTextRect = { 574,481,rightKeyText->getWidth(),rightKeyText->getHeight() };

	const SDL_Rect saveRect = { 617,594,110,47 };
	Button* save = new Button("../../../resources/SettingScreen/Button/saveNormal.png", "../../../resources/SettingScreen/Button/saveHover.png", saveRect, nullptr);

	const SDL_Rect cancelRect = { 748,594,110,47 };
	Button* cancel = new Button("../../../resources/SettingScreen/Button/cancelNormal.png", "../../../resources/SettingScreen/Button/cancelHover.png", cancelRect, nullptr);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	bool quit = false;
	SDL_Event e;
	int active = 0;
	SDL_Point currMousePos;
	bool clicked = false;

	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		bg->render(bgRect.x, bgRect.y, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT);

		homeMenu->Update(&quit);
		homeMenu->Draw();

		playMenu->Update(&quit);
		playMenu->Draw();

		settingMenu->Update();
		settingMenu->Draw();

		scoreMenu->Update(&quit);
		scoreMenu->Draw();

		creditMenu->Update(&quit);
		creditMenu->Draw();

		upKeyText->setColor(255, 255, 255);
		downKeyText->setColor(255, 255, 255);
		leftKeyText->setColor(255, 255, 255);
		rightKeyText->setColor(255, 255, 255);
		clicked = SDL_GetMouseState(&currMousePos.x, &currMousePos.y) & SDL_BUTTON_LMASK;
		if (SDL_PointInRect(&currMousePos, &upTextRect)) {
			if (clicked)
				active = 0;
			upKeyText->setColor(255, 255, 0);
		}
		else if (SDL_PointInRect(&currMousePos, &downTextRect)) {
			if (clicked)
				active = 1;
			downKeyText->setColor(255, 255, 0);
		}
		else if (SDL_PointInRect(&currMousePos, &leftTextRect)) {
			if (clicked)
				active = 2;
			leftKeyText->setColor(255, 255, 0);
		}
		else if (SDL_PointInRect(&currMousePos, &rightTextRect)) {
			if (clicked)
				active = 3;
			rightKeyText->setColor(255, 255, 0);
		}
		else if (clicked) {
			active = -1;
		}
		switch (active) {
		case 0:
			upKeyText->setColor(255, 255, 0);
			break;
		case 1:
			downKeyText->setColor(255, 255, 0);
			break;
		case 2:
			leftKeyText->setColor(255, 255, 0);
			break;
		case 3:
			rightKeyText->setColor(255, 255, 0);
			break;
		default:
			break;

		}

		upKeyText->render(upTextRect.x, upTextRect.y, NULL, upTextRect.w, upTextRect.h);
		downKeyText->render(downTextRect.x, downTextRect.y, NULL, downTextRect.w, downTextRect.h);
		leftKeyText->render(leftTextRect.x, leftTextRect.y, NULL, leftTextRect.w, leftTextRect.h);
		rightKeyText->render(rightTextRect.x, rightTextRect.y, NULL, rightTextRect.w, rightTextRect.h);

		if (temporarySetting != globalSetting) {
			save->Update();
			save->Draw();
			cancel->Update();
			cancel->Draw();
			if (SDL_PointInRect(&currMousePos, &saveRect) && clicked) {
				globalSetting = temporarySetting;
				Mix_VolumeMusic(globalSetting.volume);
			}
			else if (SDL_PointInRect(&currMousePos, &cancelRect) && clicked) {
				temporarySetting = globalSetting;

				upKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.up), WHITE, font);
				upTextRect.w = upKeyText->getWidth();
				upTextRect.h = upKeyText->getHeight();

				downKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.down), WHITE, font);
				downTextRect.w = downKeyText->getWidth();
				downTextRect.h = downKeyText->getHeight();

				leftKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.left), WHITE, font);
				leftTextRect.w = leftKeyText->getWidth();
				leftTextRect.h = leftKeyText->getHeight();

				rightKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.right), WHITE, font);
				rightTextRect.w = rightKeyText->getWidth();
				rightTextRect.h = rightKeyText->getHeight();
			}
		}

		ImGui::SetNextWindowSize({ 200,200 }, ImGuiCond_Once);
		ImGui::SetNextWindowPos({ 550,125 }, ImGuiCond_Once);
		{
			ImGui::Begin("Volume", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing);
			if (ImGuiKnobs::KnobInt("Volume1Knob", &temporarySetting.volume, 0, 100, 1, "%d", ImGuiKnobVariant_Wiper, 70, ImGuiKnobFlags_NoTitle)) {
				Mix_VolumeMusic(temporarySetting.volume);
			}
			ImGui::End();
		}


		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_WINDOWEVENT:
				cout << "window event" << endl;
				break;

			case SDL_KEYDOWN:
				switch (active) {
				case 0:
					temporarySetting.up = static_cast<SDL_KeyCode>(e.key.keysym.sym);
					upKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.up), WHITE, font);
					upTextRect.w = upKeyText->getWidth();
					upTextRect.h = upKeyText->getHeight();
					break;
				case 1:
					temporarySetting.down = static_cast<SDL_KeyCode>(e.key.keysym.sym);
					downKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.down), WHITE, font);
					downTextRect.w = downKeyText->getWidth();
					downTextRect.h = downKeyText->getHeight();
					break;
				case 2:
					temporarySetting.left = static_cast<SDL_KeyCode>(e.key.keysym.sym);
					leftKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.left), WHITE, font);
					leftTextRect.w = leftKeyText->getWidth();
					leftTextRect.h = leftKeyText->getHeight();
					break;
				case 3:
					temporarySetting.right = static_cast<SDL_KeyCode>(e.key.keysym.sym);
					rightKeyText->loadFromRenderedText(SDL_GetKeyName(temporarySetting.right), WHITE, font);
					rightTextRect.w = rightKeyText->getWidth();
					rightTextRect.h = rightKeyText->getHeight();
					break;
				default:
					break;
				}
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	delete bg;
	delete homeMenu;
	delete playMenu;
	delete settingMenu;
	delete scoreMenu;
	delete creditMenu;
	delete upKeyText;
	delete downKeyText;
	delete leftKeyText;
	delete rightKeyText;
	delete save;
	delete cancel;

	// Clean up font
	TTF_CloseFont(font);

}

void Application::creditScreen() {
	const SDL_Rect bgRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	LTexture* bg = new LTexture(gRenderer, "../../../resources/CreditScreen/Background/bg.png");

	const SDL_Rect homeMenuRect = { 68,32,-1,-1 };
	Button* homeMenu = new Button("../../../resources/CreditScreen/Button/homeMenuNormal.png", "../../../resources/CreditScreen/Button/homeMenuHover.png", homeMenuRect, &mainScreen);

	const SDL_Rect playMenuRect = { 172,32,-1,-1 };
	Button* playMenu = new Button("../../../resources/CreditScreen/Button/playMenuNormal.png", "../../../resources/CreditScreen/Button/playMenuHover.png", playMenuRect, &levelScreen);

	const SDL_Rect settingMenuRect = { 281,32,-1,-1 };
	Button* settingMenu = new Button("../../../resources/CreditScreen/Button/settingMenuNormal.png", "../../../resources/CreditScreen/Button/settingMenuHover.png", settingMenuRect, &settingScreen);

	const SDL_Rect scoreMenuRect = { 422,32,-1,-1 };
	Button* scoreMenu = new Button("../../../resources/CreditScreen/Button/scoreMenuNormal.png", "../../../resources/CreditScreen/Button/scoreMenuHover.png", scoreMenuRect, &leaderboardScreen);

	const SDL_Rect creditMenuRect = { 534,32,-1,-1 };
	Button* creditMenu = new Button("../../../resources/CreditScreen/Button/creditMenuHover.png", "../../../resources/CreditScreen/Button/creditMenuHover.png", creditMenuRect, nullptr);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	bool quit = false;
	SDL_Event e;

	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		bg->render(bgRect.x, bgRect.y, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT);

		homeMenu->Update(&quit);
		homeMenu->Draw();

		playMenu->Update(&quit);
		playMenu->Draw();

		settingMenu->Update(&quit);
		settingMenu->Draw();

		scoreMenu->Update(&quit);
		scoreMenu->Draw();

		creditMenu->Update();
		creditMenu->Draw();
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			case SDL_WINDOWEVENT:
				cout << "window event" << endl;
				break;
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	delete bg;
	delete homeMenu;
	delete playMenu;
	delete settingMenu;
	delete scoreMenu;
	delete creditMenu;
}

void Application::levelScreen() {
	const SDL_Rect bgRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	LTexture* bg = new LTexture(gRenderer, "../../../resources/LevelScreen/Background/bg.png");

	const SDL_Rect homeMenuRect = { 68,32,-1,-1 };
	Button* homeMenu = new Button("../../../resources/LevelScreen/Button/homeMenuNormal.png", "../../../resources/LevelScreen/Button/homeMenuHover.png", homeMenuRect, &mainScreen);

	const SDL_Rect playMenuRect = { 172,32,-1,-1 };
	Button* playMenu = new Button("../../../resources/LevelScreen/Button/playMenuHover.png", "../../../resources/LevelScreen/Button/playMenuHover.png", playMenuRect, nullptr);

	const SDL_Rect settingMenuRect = { 281,32,-1,-1 };
	Button* settingMenu = new Button("../../../resources/LevelScreen/Button/settingMenuNormal.png", "../../../resources/LevelScreen/Button/settingMenuHover.png", settingMenuRect, &settingScreen);

	const SDL_Rect scoreMenuRect = { 422,32,-1,-1 };
	Button* scoreMenu = new Button("../../../resources/LevelScreen/Button/scoreMenuNormal.png", "../../../resources/LevelScreen/Button/scoreMenuHover.png", scoreMenuRect, &leaderboardScreen);

	const SDL_Rect creditMenuRect = { 534,32,-1,-1 };
	Button* creditMenu = new Button("../../../resources/LevelScreen/Button/creditMenuNormal.png", "../../../resources/LevelScreen/Button/creditMenuHover.png", creditMenuRect, &creditScreen);

	const SDL_Rect levelCityRect = { 68,145,288,154 };
	const SDL_Rect levelForestRect = { 428,145,288,154 };
	LTexture* levelCity = new LTexture(gRenderer, "../../../resources/LevelScreen/Button/levelCity.png");
	LTexture* levelForest = new LTexture(gRenderer, "../../../resources/LevelScreen/Button/levelForest.png");

	const SDL_Rect mainRect = { 80,383,68,30 };
	Button* mainn = new Button("../../../resources/LevelScreen/Button/mainNormal.png", "../../../resources/LevelScreen/Button/mainHover.png", mainRect, &mainScreen);

	const SDL_Rect scoreRect = { 152,383,68,30 };
	Button* score = new Button("../../../resources/LevelScreen/Button/scoreNormal.png", "../../../resources/LevelScreen/Button/scoreHover.png", scoreRect, &leaderboardScreen);

	const SDL_Rect playRect = { 224,383,68,30 };
	Button* play = new Button("../../../resources/LevelScreen/Button/playNormal.png", "../../../resources/LevelScreen/Button/playHover.png", playRect, nullptr);

	const SDL_Rect settingRect = { 291,383,68,30 };
	Button* setting = new Button("../../../resources/LevelScreen/Button/settingNormal.png", "../../../resources/LevelScreen/Button/settingHover.png", settingRect, &settingScreen);

	const SDL_Rect levelModeRect = { 79,503,167,60 };
	Button* levelMode = new Button("../../../resources/LevelScreen/Button/levelNormal.png", "../../../resources/LevelScreen/Button/levelHover.png", levelModeRect, nullptr);

	const SDL_Rect endlessModeRect = { 279,503,167,60 };
	Button* endlessMode = new Button("../../../resources/LevelScreen/Button/endlessNormal.png", "../../../resources/LevelScreen/Button/endlessHover.png", endlessModeRect, nullptr);

	const SDL_Rect playButtonRect = { 617,595,110,47 };
	Button* playButton = new Button("../../../resources/LevelScreen/Button/playButtonNormal.png", "../../../resources/LevelScreen/Button/playButtonHover.png", playButtonRect, nullptr);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	SDL_Point currMousePos;
	int level = 0;
	int mode = 0;
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		bg->render(bgRect.x, bgRect.y, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT);

		homeMenu->Update(&quit);
		homeMenu->Draw();

		playMenu->Update();
		playMenu->Draw();

		settingMenu->Update(&quit);
		settingMenu->Draw();

		scoreMenu->Update(&quit);
		scoreMenu->Draw();

		creditMenu->Update(&quit);
		creditMenu->Draw();

		levelCity->render(levelCityRect.x, levelCityRect.y, nullptr, levelCityRect.w, levelCityRect.h);
		levelForest->render(levelForestRect.x, levelForestRect.y, nullptr, levelForestRect.w, levelForestRect.h);

		mainn->Update(&quit);
		mainn->Draw();

		score->Update(&quit);
		score->Draw();

		play->Update();
		play->Draw();

		setting->Update(&quit);
		setting->Draw();

		levelMode->Update();
		levelMode->Draw();

		endlessMode->Update();
		endlessMode->Draw();

		playButton->Update();
		playButton->Draw();

		bool clicked = SDL_GetMouseState(&currMousePos.x, &currMousePos.y);
		if (SDL_PointInRect(&currMousePos, &levelCityRect)) {
			if (clicked)
				level = 0;
			highlightRect(levelCityRect);
		}
		if (SDL_PointInRect(&currMousePos, &levelForestRect)) {
			if (clicked)
				level = 1;
			highlightRect(levelForestRect);
		}
		if (SDL_PointInRect(&currMousePos, &levelModeRect)) {
			if (clicked)
				mode = 0;
		}

		if (SDL_PointInRect(&currMousePos, &endlessModeRect)) {
			if (clicked)
				mode = 1;
		}

		if (SDL_PointInRect(&currMousePos, &playButtonRect) || SDL_PointInRect(&currMousePos, &playRect)) {
			if (clicked) {
				quit = true;
				functionStack.push(VoidFunction(reinterpret_cast<void*>(&game), { level,mode }));
			}

		}

		if (level == 0) {
			highlightRect(levelCityRect);
		}
		else {
			highlightRect(levelForestRect);
		}

		if (mode == 0) {
			levelMode->setAlwaysActiveState(1);
			endlessMode->setAlwaysActiveState(-1);
		}
		else {
			levelMode->setAlwaysActiveState(-1);
			endlessMode->setAlwaysActiveState(1);
		}

		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			}
		}
		ImGui::Render();
		SDL_RenderSetScale(gRenderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	delete bg;
	delete homeMenu;
	delete playMenu;
	delete settingMenu;
	delete scoreMenu;
	delete creditMenu;
	delete levelCity;
	delete levelForest;
	delete mainn;
	delete score;
	delete play;
	delete setting;
	delete levelMode;
	delete endlessMode;
	delete playButton;
}


void Application::mainScreen() {
	//LTexture* bg = new LTexture(gRenderer, "../../../resources/MainScreen/Background/bg.png");
	LTexture* bg_1 = new LTexture(gRenderer, "../../../resources/MainScreen/Background/1.png");
	LTexture* bg_2 = new LTexture(gRenderer, "../../../resources/MainScreen/Background/2.png");
	LTexture* bg_3 = new LTexture(gRenderer, "../../../resources/MainScreen/Background/3.png");
	LTexture* bg_4 = new LTexture(gRenderer, "../../../resources/MainScreen/Background/4.png");
	LTexture* title = new LTexture(gRenderer, "../../../resources/MainScreen/Background/title.png");
	vector<LTexture*> v;
	v.push_back(bg_1);
	v.push_back(bg_2);
	v.push_back(bg_3);
	v.push_back(bg_4);
	ParallaxBackground parallaxBG = ParallaxBackground(gRenderer, v);
	const SDL_Rect playButtonDestRect = { SCREEN_WIDTH / 2 - 200 / 2,378,200,100 };
	const SDL_Rect loadButtonDestRect = { SCREEN_WIDTH / 2 - 200 / 2,510,200,100 };
	const SDL_Rect quitButtonDestRect = { 1145,19,36,43 };
	SDL_Rect bgRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	Button* playButton = new Button("../../../resources/MainScreen/Button/playNormal.png", "../../../resources/MainScreen/Button/playHover.png", playButtonDestRect, &levelScreen);
	Button* loadButton = new Button("../../../resources/MainScreen/Button/loadNormal.png", "../../../resources/MainScreen/Button/loadHover.png", loadButtonDestRect, &game, { 0,0,true });
	Button* quitButton = new Button("../../../resources/MainScreen/Button/quitNormal.png", "../../../resources/MainScreen/Button/quitHover.png", quitButtonDestRect, &quitGame);

	//Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	//Mix_Music* music = Mix_LoadMUS("../../../resources/Music/menu.mp3");
	//Mix_PlayMusic(music, 100);
	//Mix_VolumeMusic(globalSetting.volume );
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		SDL_RenderClear(gRenderer);
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &SCREEN);
		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		//bg->render(bgRect.x, bgRect.y, nullptr, SCREEN_WIDTH, SCREEN_HEIGHT);
		parallaxBG.update(40.0f);
		parallaxBG.render();
		playButton->Update(&quit);
		playButton->Draw();
		loadButton->Update(&quit);
		loadButton->Draw();
		title->render(SCREEN_WIDTH / 2 - 530 / 2 * 1.5, 200, NULL, 530.0 * 1.5, 65.0 * 1.5);
		quitButton->Update(&quit);
		quitButton->Draw();
		if (SDL_PollEvent(&e)) {
			ImGui_ImplSDL2_ProcessEvent(&e);
			switch (e.type) {

			case SDL_QUIT:
				quit = true;
				quitGame();
				break;
			}
		}
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(gRenderer);
	}
	//delete bg;
	delete bg_1;
	delete bg_2;
	delete bg_3;
	delete bg_4;
	delete title;
	delete playButton;
	delete quitButton;
	delete loadButton;
}

Application::Application() {
}
void Application::run(){
	init();
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	Mix_Music* music = Mix_LoadMUS("../../../resources/Music/music.mp3");
	Mix_PlayMusic(music, 100);
	Mix_VolumeMusic(globalSetting.volume);
	functionStack.push(&mainScreen);
	while (!functionStack.empty()) {
		VoidFunction currActiveFunction = functionStack.top();
		if (currActiveFunction.args.size() == 0) {
			typedef void (*FunctionPtr)();
			FunctionPtr func = reinterpret_cast<FunctionPtr>(currActiveFunction.func);
			func();
		}
		else {
			typedef void (*FunctionPtr2)(const vector<int>&);
			FunctionPtr2 func = reinterpret_cast<FunctionPtr2>(currActiveFunction.func);
			func(currActiveFunction.args);
		}
		//break;
	}
}