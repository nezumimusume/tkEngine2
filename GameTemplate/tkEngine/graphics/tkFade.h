#pragma once
namespace tkEngine{
	namespace prefab{
		class CFade : public IGameObject
		{
		public:
			CFade();
			~CFade();
			bool Start() override;
			/// <summary>
			/// フェードを開始する。
			/// </summary>
			/// <param name="targetColor">ターゲットカラー</param>
			/// <param name="fadeTime">フェード時間(単位:秒)</param>
			void Start(CVector4 targetColor, float fadeTime)
			{
				m_startColor = m_currentColor;
				m_targetColor = targetColor;
				m_fadeTime = fadeTime;
				m_timer = 0.0f;
				m_isFade = true;
			}
			/// <summary>
			/// フェード中か判定
			/// </summary>
			/// <returns></returns>
			bool IsFade() const
			{
				return m_isFade;
			}
			void Update() override;
		private:
			CVector4 m_targetColor = {0.0f, 0.0f, 0.0f, 0.0f};
			CVector4 m_currentColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			CVector4 m_startColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			float m_timer = 0.0f;
			float m_fadeTime = 0.0f;
			bool m_isFade = false;
			prefab::CSpriteRender* m_spriteRender = nullptr;	//スプライトレンダラー。
		};
	}
}
