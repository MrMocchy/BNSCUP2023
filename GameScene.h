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

		//左右の視点移動
		data.viewX += data.viewScrollSpeed * Scene::DeltaTime()
			* ((data.rightKey.pressed() ? 1 : 0) + (data.leftKey.pressed() ? -1 : 0));

		for (auto& human : data.people) {
			//溺れている人の溺れ時間を進める
			if (human.drowningTime > 0) human.drowningTime += Scene::DeltaTime();
		}
		//溺れている人数が最大人数より少ない場合、ランダムに人を溺れさせる
		size_t drowningNum = data.people.count_if([](GameData::Human h) {return h.drowningTime > 0; });
		if (drowningNum < data.maxDrowningNum) {
			data.people[Random(size_t(0),data.people.size()-1)].drowningTime += Scene::DeltaTime();
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
		for (auto human : data.people)
		{
			TextureAsset(U"face{}"_fmt(human.faceIndex)).resized(human.faceIndex <= 10 ? data.faceSizeS : data.faceSizeL)
				.drawAt(human.pos + Vec2(0, 10) * Math::Sin(human.waveOffset + Time::GetMillisec() / 1000.0), ColorF{ 1.0,1.0,1.0, 1.0 - human.drowningTime / data.drownTime });
		}
	}
};

