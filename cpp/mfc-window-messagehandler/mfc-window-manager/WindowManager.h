#include <string>
#include <map>
#include <memory>

class CDesktopWindowManager
{
	private:
		class CDesktopWindowClassInternal
		{
			public:
				void OnMessage();

		};

		class CDesktopWindowInternal
		{
			public:
				void OnMessage();
		};

		struct SDesktopWindowExtras
		{
			void * pWindowClassCallback = nullptr;
			void * pWindowCallback = nullptr;
		};

	public:
		struct SDesktopWindowClassDesc
		{
			std::wstring windowClassName;
		};

		struct SDesktopWindowDesc
		{
			std::wstring windowClassName;
			std::wstring windowName;
		};

	public:
		void CreateDesktopWindowClass(SDesktopWindowClassDesc & sWindowClassDesc);
		void CreateDesktopWindow(SDesktopWindowDesc & sWindowDesc);

	private:
		std::map<std::wstring, std::shared_ptr<CDesktopWindowClassInternal>> m_mWindowClassMap;
		std::map<std::wstring, std::shared_ptr<CDesktopWindowInternal>> m_mWindowMap;
};
