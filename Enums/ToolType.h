#pragma once

enum class ToolType
{
	None,

	// General
	BaseConverter,
	Calculator,
	MarkdownEditor,

	// Productivity
	Checklist,
	TaskTracker,

	// Time
	Stopwatch,
	Timer,

	// Random
	RandomNumber,
	RandomString,

	// Filesystem
	FileSearch
};