#include "utils.h"
bool cmp(tuple<string, string, int, string, string> t1, tuple<string, string, int, string, string> t2) {
	return get<2>(t1) > get<2>(t2);
}

void quitGame() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(gRenderer);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	//Mix_Quit();
	while (!functionStack.empty())
		functionStack.pop();
	exit(0);
}
void init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	IMG_Init(IMG_INIT_JPG);
	TTF_Init();
	SDL_Init(SDL_INIT_AUDIO);
	//Mix_Init(MIX_INIT_MP3);
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
	ImGui::CreateContext();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, gRenderer);
	ImGui_ImplSDLRenderer_Init(gRenderer);

}

void saveScore(string mode, int score, string timeFormatted, string terrainType) {
	fstream outfile("score.txt", ios::out | ios::app);
	time_t t = time(0);   // get time now
	struct tm now;
	localtime_s(&now, &t);
	char tnow[100];
	strftime(tnow, sizeof(tnow), "%Y-%m-%d", &now);
	outfile << tnow << " ";
	outfile << mode << " " << score << " " << timeFormatted << " " << terrainType << endl;
	outfile.close();
}


void loadResource() {
	ResourceManager& resourceManager = ResourceManager::GetInstance();
	resourceManager.LoadTexture(gRenderer, ResourceType::Character, 32, "../../../resources/character/");

	resourceManager.LoadTexture(gRenderer, ResourceType::Spacecraft, 3, "../../../resources/vehicle/spacecraft/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Taxi, 1, "../../../resources/vehicle/taxi/");
	resourceManager.LoadTexture(gRenderer, ResourceType::BlackViper, 1, "../../../resources/vehicle/blackviper/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Ambulance, 3, "../../../resources/vehicle/ambulance/");
	resourceManager.LoadTexture(gRenderer, ResourceType::PoliceCar, 3, "../../../resources/vehicle/police/");

	resourceManager.LoadTexture(gRenderer, ResourceType::Train, 2, "../../../resources/vehicle/train/");

	//animal
	resourceManager.LoadTexture(gRenderer, ResourceType::Cat, 8, "../../../resources/animal/cat/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Fox, 8, "../../../resources/animal/fox/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Boar, 5, "../../../resources/animal/boar/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Chicken, 6, "../../../resources/animal/chicken/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Wolf, 6, "../../../resources/animal/wolf/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Cow, 5, "../../../resources/animal/cow/");

	resourceManager.LoadTexture(gRenderer, ResourceType::Explosion, 12, "../../../resources/Collision/VehicleCollision/");
	resourceManager.LoadTexture(gRenderer, ResourceType::TrafficLight, 2, "../../../resources/Road/Railway/TrafficLight/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Bridge, 1, "../../../resources/Road/River/Bridge/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Tree, 14, "../../../resources/Decorator/tree/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Flower, 14, "../../../resources/Decorator/flower/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Bush, 14, "../../../resources/Decorator/Bush/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Grass, 9, "../../../resources/Decorator/Grass/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Stone, 4, "../../../resources/Road/RollingStoneRoad/stone/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Timber, 8, "../../../resources/Road/ForestRiver/timber/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Vendor_1, 4, "../../../resources/Decorator/vendor/vendor1/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Vendor_2, 4, "../../../resources/Decorator/vendor/vendor2/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Danger_1, 1, "../../../resources/Road/ConstructionSite/danger/danger1/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Danger_2, 1, "../../../resources/Road/ConstructionSite/danger/danger2/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Cloud, 3, "../../../resources/Decorator/Cloud/");

	resourceManager.LoadTexture(gRenderer, ResourceType::SimpleRoad, 3, "../../../resources/Road/SimpleRoad/");
	resourceManager.LoadTexture(gRenderer, ResourceType::SimpleSafeRoad, 1, "../../../resources/Road/SimpleSafeRoad/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Railway, 1, "../../../resources/Road/Railway/Rail/");
	resourceManager.LoadTexture(gRenderer, ResourceType::UpperWaterLane, 14, "../../../resources/Road/River/River_Upper/");
	resourceManager.LoadTexture(gRenderer, ResourceType::LowerWaterLane, 14, "../../../resources/Road/River/River_Lower/");
	resourceManager.LoadTexture(gRenderer, ResourceType::SafeForestRoad, 1, "../../../resources/Road/SafeForestRoad/");
	resourceManager.LoadTexture(gRenderer, ResourceType::AnimalRoad, 1, "../../../resources/Road/AnimalRoad/");
	resourceManager.LoadTexture(gRenderer, ResourceType::MonsterRoad, 1, "../../../resources/Road/MonsterRoad/");
	resourceManager.LoadTexture(gRenderer, ResourceType::RollingStoneRoad, 1, "../../../resources/Road/RollingStoneRoad/");
	resourceManager.LoadTexture(gRenderer, ResourceType::ConstructionSite, 1, "../../../resources/Road/ConstructionSite/road/");


	//temporary
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_IDLE, 12, "../../../resources/Monster/Archer/idle/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ATTACK_1, 15, "../../../resources/Monster/Archer/1_atk/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ATTACK_2, 12, "../../../resources/Monster/Archer/2_atk/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ATTACK_3, 17, "../../../resources/Monster/Archer/sp_atk/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ARROW, 1, "../../../resources/Monster/Archer/projectiles_and_effects/arrow/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ARROW_EFFECT_1, 8, "../../../resources/Monster/Archer/projectiles_and_effects/arrow_hit_entangle/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ARROW_EFFECT_2, 8, "../../../resources/Monster/Archer/projectiles_and_effects/arrow_hit_poison/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ARROW_EFFECT_SPECIAL, 9, "../../../resources/Monster/Archer/projectiles_and_effects/beam_extension_effect/");
	resourceManager.LoadTexture(gRenderer, ResourceType::Archer_ARROW_EFFECT_SHOWER, 18, "../../../resources/Monster/Archer/projectiles_and_effects/arrow_shower_effect/");





}

void highlightRect(SDL_Rect rectHighlight) {
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(gRenderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);//yellow
	SDL_RenderDrawRect(gRenderer, &rectHighlight);
	SDL_SetRenderDrawColor(gRenderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}