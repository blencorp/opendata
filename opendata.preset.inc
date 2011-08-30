<?php 

/**
 * Implementation of hook_form_submit()
 */
function opendata_create_preset($preset_name) {
	// make sure preset name is alpha-numeric
	$pname = _create_preset_name($preset_name);

	// openlayers_preset_save takes a $style array
	// it is located on line 1095 of openlayers.module file 

	//module_load_include('inc', 'openlayers', 'openlayers_ui.presets');
	$preset = opendata_build_preset($pname);
	$result = openlayers_preset_save($preset);

	if ($result) {
		//drupal_set_message('Sucess: preset ' . $pname . ' created.', 'status');
		drupal_set_message(t('Success: preset !pname has been created.', array('!pname' => $pname)), 'status');

		// Clear the page cache.
		//cache_clear_all();

		return true;
	} else {
		drupal_set_message(t('Error: could not create preset !pname.', array('!pname' => $pname)), 'error');
		//drupal_set_message('Error: could not create preset ' . $pname, 'error');
		return false;
	}
}

/**
 * A clone of hook_openlayers_presets()
 */
function opendata_build_preset($pname) {

	//TODO: check to make sure we have the proper form id

	$preset = new stdClass();
	$preset->name = $pname;
	$preset->title = $pname;
	$preset->description = "A preset created by OpenData module."; 

	// create map overlay 
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
				//'zoomtolayer' => 'rtl_listing_openlayers_1', 
				'zoomtolayer' => $pname . '_openlayers_1', 
				'point_zoom_level' => '5',
			),
		),
		'default_layer' => 'dped_browser_world_light_usa',
		'layers' => array(
			'dped_browser_world_light_usa' => 'dped_browser_world_light_usa',
			'congressional_districts' => 'congressional_districts',
			'rtl_listing_openlayers_1' => 'rtl_listing_openlayers_1',
		),
		'layer_styles' => array(
			//'rtl_listing_openlayers_1' => 'dped_blue',
			$pname . '_openlayers_1' => 'dped_blue',
		),
		'layer_activated' => array(
			$pname . '_openlayers_1' => $pname . '_openlayers_1',
			//'rtl_listing_openlayers_1' => 'rtl_listing_openlayers_1',
		),
		'layer_switcher' => array(),
		'projection' => '900913', 
		'displayProjection' => '4326', 
		'styles' => array(
			'default' => 'circle-7',
			'select' => 'circle-7',
			'temporary' => 'circle-7',
		),
		'options' => NULL,
	);

	$preset->data = $map;

	return $preset;
}

/**
 * A helper function for creating a valid preset name (alpha-numeric string).
 * 
 * @param $preset_title 
 *   The preset title provided by the user
 *
 * @return 
 *   An alpha-numeric string to be used as preset name. 
 */
function _create_preset_name($preset_title) {
	$pname = strtolower($preset_title);
	$pname = str_replace(' ', '_', $pname);
	return $pname;
}


function opendata_test_preset() {

	$pname = 'od_oii_2010_school_improvement';

	//TODO: check to make sure we have the proper form id

	$preset = new stdClass();
	$preset->name = $pname;
	$preset->title = $pname;
	$preset->description = "A preset created by OpenData module."; 

	// create map overlay 
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
			'openlayers_behavior_keyboarddefaults' => array(),
			'openlayers_behavior_layerswitcher' => array(),
			'openlayers_behavior_navigation' => array(
				'zoomWheelEnabled' => 1,
			),
			'openlayers_behavior_panzoombar' => array(),
			'openlayers_behavior_zoomtolayer' => array(
				'zoomtolayer' => 'rtl_listing_openlayers_1', 
				'point_zoom_level' => '5',
			),
		),
		'default_layer' => 'dped_browser_world_light_usa',
		'layers' => array(
			'dped_browser_world_light_usa' => 'dped_browser_world_light_usa',
			'congressional_districts' => 'congressional_districts',
			'rtl_listing_openlayers_1' => 'rtl_listing_openlayers_1',
		),
		'layer_styles' => array(
			'rtl_listing_openlayers_1' => 'dped_blue',
		),
		'layer_activated' => array(
			'rtl_listing_openlayers_1' => 'rtl_listing_openlayers_1',
		),
		'layer_switcher' => array(),
		'projection' => '900913', 
		'displayProjection' => '4326', 
		'styles' => array(
			'default' => 'circle-7',
			'select' => 'circle-7',
			'temporary' => 'circle-7',
		),
		'options' => NULL,
	);

	$preset->data = $map;

	$result = openlayers_preset_save($preset);

	if ($result) {
		drupal_set_message(t('Success: preset !pname has been created.', array('!pname' => $preset->name)), 'status');
	} else {
		drupal_set_message(t('Error: could not create preset !pname.', array('!pname' => $preset->name)), 'error');
	}
}
