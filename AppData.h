#pragma once

struct AppData {

	bool isTitle = true;
	bool isInGame = false;
	bool isGameEnd = false;
	bool isGameOver = false;
	bool isGameClear = false;

	Effect effect;

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
		double drownTime;
		bool splash = false;
		bool isSaved = false;
		bool isDead = false;
	};
	Array<Human> people;
	const size_t peopleNum = 100;
	const size_t faceNum = 24;
	// 絵文字のデザインによって異なる顔のサイズを揃えるための定数
	double faceSizeS = 30.0;
	double faceSizeL = 40.0;
	double swimRingSize = 30.0;
	//溺れ
	size_t maxDrowningNum = 2;

	void Init() {
		ClearPrint();
		isTitle = true;
		isInGame = false;
		isGameEnd = false;
		isGameOver = false;
		isGameClear = false;
		viewX = 0;
		effect.clear();
		people.clear();

		// 顔文字の登録
		Array<String> faces{ U"👶",U"🧒",U"👦",U"👧",U"🧑",U"👨",U"🧔",U"👩",U"🧓",U"👴",U"👵",U"👨‍🦱",U"👱",U"👨‍🦰",U"👨‍🦳",U"👨‍🦲",U"👩‍🦰",U"🧑‍🦰",U"👩‍🦱",U"🧑‍🦱",U"👩‍🦳",U"🧑‍🦳",U"👩‍🦲",U"🧑‍🦲" };
		for (auto i : step(faceNum))
		{
			TextureAsset::Register(U"face{}"_fmt(i), Emoji{ faces[i] });
		}
		TextureAsset::Register(U"splash", Emoji{ U"💦" });

		// 人の作成
		for (auto i : step(peopleNum))
		{
			Human human;
			human.pos = RandomVec2(sea);
			human.faceIndex = Random(faceNum - 1);
			human.waveOffset = Random(0.0, 360.0_deg);
			human.drowningTime = 0;

			//溺れるまでの時間
			switch (human.faceIndex) {
			case 0:
				//赤ちゃん
				human.drownTime = 5.0;
				break;
			case 8:
			case 9:
			case 10:
			case 14:
			case 15:
			case 20:
			case 21:
			case 22:
			case 23:
				//老人
				human.drownTime = 10.0;
				break;
			default:
				//若者
				human.drownTime = 15.0;
				break;
			}

			human.splash = Random(0.0, 1.0) < 0.5;

			people.push_back(human);
			//people.push_back({ Vec2(50*i,300), i, Random(0.0, 360.0_deg),0,5});
		}
	}
};

struct SplashEffect : IEffect {
	double duration = 1.0;
	double size = 0.3;
	double angle = 0.0;
	Vec2 pos;

	explicit SplashEffect(const Vec2& _pos) :
		pos(_pos) {
		angle = Random(0.0, 360.0_deg);
	}

	bool update(double t) override {
		size = size * (duration - t/5);
		TextureAsset(U"splash").scaled(size).rotated(angle).drawAt(pos);
		return t < duration;
	}

};
