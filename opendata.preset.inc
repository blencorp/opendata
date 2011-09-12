<?php 

/**
 * Implementation of hook_form_submit()
 * 
 * openlayers_preset_save takes a $style array
 * it is located on line 1095 of openlayers.module file 
 * 
 * A clone of hook_openlayers_presets()
 similar code in openlayers/modules/openlayers_ui/includes/openlayers_ui.presets.inc 
 on line 464
 */
function opendata_create_preset($preset_name, $preset_type) {
	// make sure preset name is alpha-numeric
	$pname = strtolower($preset_name);
	$pname = str_replace(' ', '_', $pname);

	if ($preset_type == 'detail') {
		$pname = $pname . '_detail';
	}

	// get the map data array
	$map = _opendata_build_map($pname, $preset_type);

	$preset = new stdClass();
	$preset->disabled = FALSE;
	$preset->api_version = 1;
	$preset->name = $pname;
	$preset->title = $pname;
	$preset->description = "A preset created by OpenData module."; 
	$preset->data = $map;

	$result = openlayers_preset_save($preset);

	if ($result) {
		//drupal_set_message(t('Map saved.'));
		return true;
	} else {
		drupal_set_message(t('Error: could not save OL preset !pname.', array('!pname' => $pname)), 'error');
		return false;
	}
}

function _opendata_build_map($pname, $preset_type) {
	if ($preset_type == 'detail') {
		$height = '200px';
		$centerpoint = '-96.767578121148, 37.926867600188';
		$zoom = '4';
		$dot_style = 'dped_blue_large';
	} else {
		$height = '500px';
		$centerpoint = '-101.07421874601, 39.368279147837';
		$zoom = '2';
		$dot_style = 'dped_blue';
	}
	$map = array(
		'width' => 'auto',
		'height' => $height,
		'image_path' => 'profiles/ngp_deped/openlayers/dark/',
		'css_path' => '',
		'proxy_host' => '',
		'center' => array( 
			'initial' => array(
				'centerpoint' => $centerpoint,
				'zoom' => $zoom,
			),
			'restrict' => array(
				'restrictextent' => 0, 
				'restrictedExtent' => '',
			),
		),
		'behaviors' => array(
			'openlayers_behavior_attribution' => array(),
			'openlayers_behavior_popup' => array(),
			'openlayers_behavior_fullscreen' => array(),
			'openlayers_behavior_keyboarddefaults' => array(),
			'openlayers_behavior_navigation' => array(
				'zoomWheelEnabled' => 1,
			),
			'openlayers_behavior_panzoombar' => array(),
			'openlayers_behavior_zoomtolayer' => array(
				'zoomtolayer' => $pname . '_openlayers_1', 
				'point_zoom_level' => '5',
			),
		),
		'default_layer' => 'dped_browser_world_light_usa',
		'layers' => array(
			'dped_browser_world_light_usa' => 'dped_browser_world_light_usa',
			$pname . '_openlayers_1' => $pname . '_openlayers_1',
		),
		'layer_styles' => array(
			$pname . '_openlayers_1' => 'dped_blue',
		),
		'layer_activated' => array(
			$pname . '_openlayers_1' => $pname . '_openlayers_1',
		),
		'layer_switcher' => array(),
		'projection' => '900913', 
		'displayProjection' => '4326', 
		'styles' => array(
			'default' => $dot_style,
			'select' => $dot_style,
			'temporary' => $dot_style,
		),
		'options' => NULL,
	);

	return $map;
}
