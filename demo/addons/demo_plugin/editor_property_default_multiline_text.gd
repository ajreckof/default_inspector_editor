@tool
extends EditorPlugin

var inspector_plugin : EditorInspectorPlugin
const EditorPropertyDefaultMultilineTextInspector = preload("res://addons/editor_property_default_multiline_text/editor_property_default_multiline_text_inspector.gd")

func _enable_plugin() -> void:
	inspector_plugin = EditorPropertyDefaultMultilineTextInspector.new()
	add_inspector_plugin(inspector_plugin)
	


func _disable_plugin() -> void:
	inspector_plugin = null
