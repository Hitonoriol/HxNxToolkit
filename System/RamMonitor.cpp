#include "RamMonitor.h"
#include "Windows.h"

RamMonitor::RamMonitor(QWidget *parent)
	: Component(parent, ToolType::RamMonitor)
{
	ui.setupUi(this);
	connect(&updateTimer, &QTimer::timeout, this, &RamMonitor::Update);
	Update();
	updateTimer.start(1'000);
}

RamMonitor::~RamMonitor()
{}

void RamMonitor::Update()
{
	MEMORYSTATUSEX memStats{};
	memStats.dwLength = sizeof(MEMORYSTATUSEX);

	GlobalMemoryStatusEx(&memStats);

	ui.StatsText->setText(QString("%1 GiB / %2 GiB (%3%)")
		.arg((memStats.ullTotalPhys - memStats.ullAvailPhys) / 1024.0f / 1024.0f / 1024.0f, 0, 'f', 1)
		.arg(memStats.ullTotalPhys / 1024.0f / 1024.0f / 1024.0f, 0, 'f', 1)
		.arg(memStats.dwMemoryLoad));
}