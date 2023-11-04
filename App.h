﻿#pragma once

class App
{
public:

	AppData data;

	void update()
	{
		Transformer2D transform(Mat3x2::Translate(-data.viewX, 0.0), TransformCursor::Yes);

		//左右の視点移動
		data.viewX += data.viewScrollSpeed * Scene::DeltaTime()
			* ((data.rightKey.pressed() ? 1 : 0) + (data.leftKey.pressed() ? -1 : 0));

		//溺れている人の溺れ時間を進める
		for (auto& human : data.people) {
			if (human.drowningTime > 0) human.drowningTime += Scene::DeltaTime();

			if (human.drowningTime > data.drownTime && (not human.isSaved && not human.isDead)) {
				//ゲームオーバー画面へ遷移
				Print << U"GameOver";
				human.isDead = true;
			}
		}
		//溺れている人数が最大人数より少ない場合、ランダムに人を溺れさせる
		size_t drowningNum = data.people.count_if([](AppData::Human h) {return (not h.isSaved && not h.isDead) && h.drowningTime > 0; });
		if (drowningNum < data.maxDrowningNum) {
			data.people[Random(size_t(0),data.people.size()-1)].drowningTime += Scene::DeltaTime();
		}

		for (auto& human : data.people)
		{
			if (human.drowningTime == 0 || human.isSaved || human.isDead)continue;
			//溺れている人の救助判定
			if (MouseL.down() && (human.pos + Vec2(0, 10) * Math::Sin(human.waveOffset + Time::GetMillisec() / 1000.0) - Cursor::PosF()).length() < 40) {
				Print << U"Saved";
				human.isSaved = true;
			}
		}
	}

	void draw()
	{
		Transformer2D transform(Mat3x2::Translate(-data.viewX, 0.0),TransformCursor::Yes);

		//背景描画
		data.sky.draw(Palette::Skyblue);
		data.sea.draw(Palette::Deepskyblue);
		data.beach.draw(Palette::Orange);

		//人描画
		for (auto& human : data.people)
		{
			//波アニメーション
			Vec2 pos = human.pos + Vec2(0, 10) * Math::Sin(human.waveOffset + Time::GetMillisec() / 1000.0);

			//浮き輪奥
			if (human.isSaved)
			{
				Transformer2D t(Mat3x2::Scale(1, 0.6,pos));
				for (auto i : step(4)) {
					Circle{ pos,data.swimRingSize }.drawArc(-90_deg + 45_deg * i, 45_deg,data.swimRingSize / 3, 0, i % 2 == 0 ? Palette::White : Palette::Red);
				}
			}

			TextureAsset(U"face{}"_fmt(human.faceIndex)).resized(human.faceIndex <= 10 ? data.faceSizeS : data.faceSizeL)
				.drawAt(pos, ColorF{ 1.0,1.0,1.0,human.isSaved ? 1.0 : 1.0 - human.drowningTime / data.drownTime });

			//浮き輪手前
			if (human.isSaved)
			{
				Transformer2D t(Mat3x2::Scale(1, 0.6, pos));
				for (auto i : step(4)) {
					Circle{ pos,data.swimRingSize }.drawArc(+90_deg + 45_deg * i, 45_deg, data.swimRingSize / 3, 0, i % 2 == 0 ? Palette::White : Palette::Red);
				}
			}
		}
	}
};
