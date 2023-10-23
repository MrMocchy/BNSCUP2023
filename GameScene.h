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

		data.viewX += data.viewScrollSpeed * Scene::DeltaTime()
			* ((data.rightKey.pressed() ? 1 : 0) + (data.leftKey.pressed() ? -1 : 0));
		
	}

	void draw() const override
	{
		auto& data = getData();
		Transformer2D transform(Mat3x2::Translate(-data.viewX, 0.0));

		data.sky.draw(Arg::left = Palette::Skyblue, Arg::right = Palette::White);
		data.sea.draw(Arg::left = Palette::Deepskyblue, Arg::right = Palette::White);
		data.beach.draw(Arg::left = Palette::Orange, Arg::right = Palette::White);
	}
};

