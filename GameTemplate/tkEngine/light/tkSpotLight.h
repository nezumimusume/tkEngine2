#pragma once

#include "tkEngine/graphics/tkLight.h"
#include "tkEngine/light/tkLightBase.h"

namespace tkEngine {
	namespace prefab {
		/// <summary>
		/// スポットライト。
		/// </summary>
		class CSpotLight : public CLightBase {
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			CSpotLight();
			/// <summary>
			/// デストラクタ。
			/// </summary>
			~CSpotLight();
			/// <summary>
			/// スポットライトのパラメータを更新する。
			/// </summary>
			/// <param name="position">スポットライトの座標</param>
			/// <param name="color">スポットライトのカラー</param>
			/// <param name="direction">照射方向</param>
			/// <param name="angle">照射角度</param>
			/// <param name="length">照射距離</param>
			/// <param name="attnPow">
			/// 減衰係数。
			/// 1.0なら線形の減衰。1.0より大きな数値を設定すると、非線形な減衰になります。
			/// </param>
			void UpdateParam(
				const CVector3& position, 
				const CVector3& color, 
				const CVector3& direction,
				float length,
				float attnPow = 1.0f
			);
		private:
			/// <summary>
			/// 更新開始時に呼ばれる処理。
			/// </summary>
			/// <returns></returns>
			bool StartSub() override final;
			/// <summary>
			/// 更新処理。
			/// </summary>
			void Update() override final;
		private:
			SSpotLight light;		//スポットライト。
		};
	}
}