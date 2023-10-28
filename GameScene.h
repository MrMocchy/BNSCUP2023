#pragma once

class GameScene : public App::Scene
{
public:
	GameScene(const InitData& init)
		: IScene{ init }
	{
	}

	void update() override
	{
		auto& data = getData();
		Transformer2D transform(Mat3x2::Translate(-data.viewX, 0.0));

		//左右の視点移動
		data.viewX += data.viewScrollSpeed * Scene::DeltaTime()
			* ((data.rightKey.pressed() ? 1 : 0) + (data.leftKey.pressed() ? -1 : 0));

		//溺れている人の溺れ時間を進める
		for (auto& human : data.people) {
			if (human.drowningTime > 0) human.drowningTime += Scene::DeltaTime();

			if (human.drowningTime > data.drownTime && not human.isSaved) {
				//ゲームオーバー画面へ遷移
				Print << U"GameOver";
				human.isSaved = true;
			}
		}
		//溺れている人数が最大人数より少ない場合、ランダムに人を溺れさせる
		size_t drowningNum = data.people.count_if([](GameData::Human h) {return not h.isSaved && h.drowningTime > 0; });
		if (drowningNum < data.maxDrowningNum) {
			data.people[Random(size_t(0),data.people.size()-1)].drowningTime += Scene::DeltaTime();
		}

		for (auto& human : data.people)
		{
			if (human.drowningTime == 0)continue;
			//溺れている人の救助判定
			if (MouseL.down() && (human.pos - Cursor::PosF()).length() < 40) {
				Print << U"Saved";
				human.isSaved = true;
			}
		}
	}

	void draw() const override
	{
		auto& data = getData();
		Transformer2D transform(Mat3x2::Translate(-data.viewX, 0.0));

		//背景描画
		data.sky.draw(Palette::Skyblue);
		data.sea.draw(Palette::Deepskyblue);
		data.beach.draw(Palette::Orange);

		//人描画
		for (auto& human : data.people)
		{
			TextureAsset(U"face{}"_fmt(human.faceIndex)).resized(human.faceIndex <= 10 ? data.faceSizeS : data.faceSizeL)
				.drawAt(human.pos + Vec2(0, 10) * Math::Sin(human.waveOffset + Time::GetMillisec() / 1000.0), ColorF{ 1.0,1.0,1.0, 1.0 - human.drowningTime / data.drownTime });
		}
	}
};

