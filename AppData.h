#pragma once

struct AppData {

	bool isTitle = true;
	bool isInGame = false;
	bool isGameEnd = false;
	bool isGameOver = false;
	bool isGameClear = false;

	Rect sky{ Arg::center = Point(600,50), 1200, 100};
	Rect sea{ Arg::center = Point(600, 250), 1200, 300 };
	Rect beach{ Arg::center = Point(600,500), 1200, 200};

	double viewX = 0.0;

	InputGroup rightKey = InputGroup{ KeyRight, KeyD };
	InputGroup leftKey = InputGroup{ KeyLeft, KeyA };
	double viewScrollSpeed = 300.0;

	struct Human {
		Vec2 pos;
		size_t faceIndex;
		double waveOffset;
		double drowningTime;
		bool isSaved = false;
		bool isDead = false;
	};
	Array<Human> people;
	const size_t peopleNum = 100;
	size_t faceNum = 0;
	// 絵文字のデザインによって異なる顔のサイズを揃えるための定数
	double faceSizeS = 30.0;
	double faceSizeL = 40.0;
	double swimRingSize = 30.0;
	//溺れ
	size_t maxDrowningNum = 2;
	double drownTime = 5.0;

	AppData() {
		// 顔文字の登録
		Array<String> faces{ U"👶",U"🧒",U"👦",U"👧",U"🧑",U"👨",U"🧔",U"👩",U"🧓",U"👴",U"👵",U"👨‍🦱",U"👱",U"👨‍🦰",U"👨‍🦳",U"👨‍🦲",U"👩‍🦰",U"🧑‍🦰",U"👩‍🦱",U"🧑‍🦱",U"👩‍🦳",U"🧑‍🦳",U"👩‍🦲",U"🧑‍🦲" };
		faceNum = faces.size();
		for (auto i : step(faceNum))
		{
			TextureAsset::Register(U"face{}"_fmt(i), Emoji{ faces[i] });
		}

		// 人の作成
		for (auto i : step(peopleNum))
		{
			people.push_back({ RandomVec2(sea), Random(faceNum-1), Random(0.0, 360.0_deg),0.0 });
			//people.push_back({ Vec2(50*i,300), i, Random(0.0, 360.0_deg)});
		}
	}
};
