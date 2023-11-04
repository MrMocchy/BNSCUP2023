#pragma once

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
		
		for (auto& human : data.people) {
			if (human.drowningTime > 0 && not human.isSaved)
			{
				//溺れている人の溺れ時間を進める
				human.drowningTime += Scene::DeltaTime();

				//水しぶきのエフェクト作成
				if (human.splash && human.drowningTime*2 - Floor(human.drowningTime*2) < Scene::DeltaTime()*2) {
					data.effect.add<SplashEffect>(human.pos+RandomVec2()*20);
				}
			}

			if (human.drowningTime > human.drownTime && (not human.isSaved && not human.isDead)) {
				//ゲームオーバー画面へ遷移
				//Print << U"GameOver";
				human.isDead = true;
				data.isInGame = false;
				data.isGameEnd = true;
				data.isGameOver = true;
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
				//Print << U"Saved";
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
				.drawAt(pos, ColorF{ 1.0,1.0,1.0,human.isSaved ? 1.0 : 1.0 - human.drowningTime / human.drownTime });

			//浮き輪手前
			if (human.isSaved)
			{
				Transformer2D t(Mat3x2::Scale(1, 0.6, pos));
				for (auto i : step(4)) {
					Circle{ pos,data.swimRingSize }.drawArc(+90_deg + 45_deg * i, 45_deg, data.swimRingSize / 3, 0, i % 2 == 0 ? Palette::White : Palette::Red);
				}
			}
		}

		data.effect.update();
	}

	void drawTitle() {
		FontAsset(U"Large")(U"LIFEGUARD").drawAt(Scene::CenterF() - Vec2(0, 150), Palette::Orangered);
		FontAsset(U"Large")(U"GAME").drawAt(Scene::CenterF() - Vec2(0, 50), Palette::Orangered);
		auto startButton = RectF{ Arg::center = Scene::CenterF() + Vec2(0,100),SizeF{180,50} };
		if (startButton.rounded(5).draw(Palette::White).drawFrame(2,Palette::Gray).mouseOver()) {
			Cursor::RequestStyle(CursorStyle::Hand);
			if(MouseL.down()) {
				data.isTitle = false;
				data.isInGame = true;
			}
		}
		FontAsset(U"Medium")(U"START").drawAt(startButton.center(), Palette::Black);

	}

	void drawGameClear() {
		drawGameEnd(U"GAME CLEAR");
	}

	void drawGameOver() {
		drawGameEnd(U"GAME OVER");
	}

	void drawGameEnd(String str) {
		FontAsset(U"Large")(str).drawAt(Scene::CenterF() - Vec2(0, 100), Palette::Orangered);
		auto titleButton = RectF{ Arg::center = Scene::CenterF() + Vec2(0,100),SizeF{180,50} };
		if (titleButton.rounded(5).draw(Palette::White).drawFrame(2, Palette::Gray).mouseOver()) {
			Cursor::RequestStyle(CursorStyle::Hand);
			if (MouseL.down()) {
				data.Init();
			}
		}
		FontAsset(U"Medium")(U"TITLE").drawAt(titleButton.center(), Palette::Black);
	}

};

