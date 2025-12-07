@tool
extends EditorInspectorPlugin



func _can_handle(object: Object) -> bool:
	return true

func _parse_property(object: Object, type: Variant.Type, name: String, hint_type: PropertyHint, hint_string: String, usage_flags: int, wide: bool) -> bool:
	if hint_type == PROPERTY_HINT_MULTILINE_TEXT :
		printt(name, hint_type, usage_flags)
		var editor := EditorPropertyDefaultMultilineText.new()
		add_property_editor(name, editor)
		return true
	return false
