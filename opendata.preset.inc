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
function opendata_create_preset($preset_name) {
	// make sure preset name is alpha-numeric
	$pname = strtolower($preset_name);
	$pname = str_replace(' ', '_', $pname);

	// get the map data array
	$map = _opendata_build_map($pname);

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

function _opendata_build_map($pname) {
	$map = array(
		'width' => 'auto',
		'height' => '500px', 
		'image_path' => 'profiles/ngp_deped/openlayers/dark/',
		'css_path' => '',
		'proxy_host' => '',
		'center' => array( 
			'initial' => array(
				'centerpoint' => '-96.767578121148, 37.926867600188',
				'zoom' => '4',
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
				'zoomtolayer' => 'od_oii_2010_cc2_openlayers_1', 
				'point_zoom_level' => '5',
			),
		),
		'default_layer' => 'dped_browser_world_light_usa',
		'layers' => array(
			'dped_browser_world_light_usa' => 'dped_browser_world_light_usa',
			$pname . '_openlayers_1' => $pname . '_openlayers_1',
			//'od_oii_2010_cc2_openlayers_1' => 'od_oii_2010_cc2_openlayers_1',
		),
		'layer_styles' => array(
			$pname . '_openlayers_1' => 'dped_blue',
			//'od_oii_2010_cc2_openlayers_1' => 'dped_blue',
		),
		'layer_activated' => array(
			$pname . '_openlayers_1' => $pname . '_openlayers_1',
			//'od_oii_2010_cc2_openlayers_1' => 'od_oii_2010_cc2_openlayers_1',
		),
		'layer_switcher' => array(),
		'projection' => '900913', 
		'displayProjection' => '4326', 
		'styles' => array(
			'default' => 'dped_blue',
			'select' => 'dped_blue',
			'temporary' => 'dped_blue',
		),
		'options' => NULL,
	);

	return $map;
}
