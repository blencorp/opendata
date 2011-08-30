<?php

/**
 * @file
 * OpenData Module
 */

/** 
 * Creates the primary form for opendata. 
 * 
 * There used to be a require_once call to include opendata.preset.inc, 
 * but we are now loading it with module_load_include right before calling 
 * opendata_create_preset(). 
 * 
 * Process: 
 *		- module calls opendata_general 
 *		- if param $form_state['storage']['summary'] is set, it calls opendata_summary 
 *		- if param $form_state['storage']['detail'] is set, it calls opendata_detail 
 *		- if param $form_state['storage']['published'] is set, it calls opendata_published
 * 
 * @param $form_state 
 *		An array holding the form data 
 * 
 * @return 
 *		A $form array with user provided values.
 *
 * @todo the office field should have a db source 
 *
 */
function opendata_general($form_state) {

    if (isset($form_state['storage']['summary']) AND !isset($form_state['storage']['detail'])) {
        return opendata_summary($form_state);
    }
    else if (isset($form_state['storage']['detail']) AND !isset($form_state['storage']['published'])) {
        return opendata_detail($form_state);
    }
    else if (isset($form_state['storage']['published'])) {
        return opendata_published($form_state);
    }

    //Basic Information
    $form['details'] = array(
			'#type' => 'fieldset',
			'#title' => t('Grant Basic Information'),
			'#tree' => TRUE,
    );
		$form['details']['data_table'] = array(
			'#type' => 'select',
			'#title' => t('Data Table (Don\'t see your table? <a href="#">Refresh</a> the list)'),
			'#options' => _opendata_get_tables(),
			'#description' => t('Please select the data table'),
    );
    $form['details']['office'] = array(
        '#type' => 'select',
        '#title' => t('Office'),
        '#options' => array(
            'OII'=>'Office of Innovation and Improvement',
            'OESE'=>'Office of Elementary and Secondary Education',
            'OPEPD'=>'Office of Planning, Evaluation and Policy Development',
            'OPE'=>'Office of Post-Secondary Education',
            'IES'=>'Institute of Education Sciences',
            'FSA'=>'Federal Student Aid',
            ),
        '#description' => t('Please select the office'),
    );
    $form['details']['fiscal_year'] = array(
        '#type' => 'select',
        '#title' => t('Fiscal Year'),
        '#options' => array(
            '2010'=>'2010',
            '2011'=>'2011',
            '2012'=>'2012',
            '2013'=>'2013',
         ),
        '#description' => t('Please select the fiscal year'),
    );
	$form['details']['title'] = array(
        '#type' => 'textfield',
		'#title' => t('Title'),
		'#description' => t('Please enter the Title'),
	);
	$form['details']['description'] = array(
		'#type' => 'textarea',
		'#title' => t('Description'),
		'#description' => t('Please enter the description of the dataset'),
	);
	$form['details']['more_info_url'] = array(
		'#type' => 'textfield',
		'#title' => t('More Info URL'),	
		'#description' => t('Please enter the more info URL'),
	);
    $form['next_summary'] = array(
        '#type' => 'submit',
        '#value' => 'Next',
    );
    
	return $form;
}

/** 
 * Creates the form that defines the listing (summary) page. 
 *
 * @todo write helper functions to guess a label for each field 
 * @todo we also need a function to guess the field for each map option field
 * @todo use a helper function to guess the column names for the summary field
 *
 * Process: 
 *	- creates an array called $step_1 to display three items from the first form: 
 *		- grant title 
 *		- data table name 
 *		- fiscal year 
 *
 *	- it calls helper function _opendata_get_tablename($tablename) to get the 
 *		actual table name of the data selected at step_1
 *	- it calls helper function _opendata_get_fields to populate the field 
 *		"Table Key" with the available columns in the data table selected.
 *	- it calls the helper function _opendata_build_summary_listing() to populate 
 *		the form with available fields from the data table (passing the name of 
 *		the data table selected at step_1
 *
 *	@param $form_state 
 *		An array holding the form data 
 *	@return 
 *		A $form array with user provided values.
 *
 */
function opendata_summary($form_state) {
    
	//Summary Page
	$step_1 = array();
	
	$step_1['title'] = $form_state['storage']['general_values']['details']['title'];              //Get title of the dataset
	$step_1['data_table'] = $form_state['storage']['general_values']['details']['data_table'];    //Get dataset table  
	$step_1['fiscal_year'] = $form_state['storage']['general_values']['details']['fiscal_year'];    //Get dataset table  
	
	//Show grant title, data table, fiscal year -- for reference
	$form['r_grant_title'] = array(
	    '#type' => 'item',
	    '#title' => t('Grant Title - ' . $step_1['title']),
	);
	$form['r_data_table'] = array(
	    '#type' => 'item',
	    '#title' => t('Data Table - ' . _opendata_get_tablename($step_1['data_table'])),
	);
	$form['r_fiscal_year'] = array(
	    '#type' => 'item',
	    '#title' => t('Fiscal Year - ' . $step_1['fiscal_year']),
	);
	
	//Other summary related fields
	$form['summary']['table_key'] = array(
	    '#type' => 'select',
	    '#title' => t('Table Key'),
	    '#options' => _opendata_get_fields($step_1['data_table']),
	    '#description' => t('Please choose the unique column'),
	);   
	$form['summary']['page_limit'] = array(
		'#type' => 'textfield',
		'#size' => 10,
		'#title' => "Page Limit",
		'#default_value' => "25",
		'#description' => t('Please enter the page limit. Enter 0 for unlimited'),
	);
	$form['summary']['csv_download'] = array(
		'#type' => 'checkbox',
		'#title' => t('CSV Download'),
		'#description' => t('Please select to allow visitors to download raw CSV file.'),
	);
				
	//Append the columns based on the selected data table
	$form[] = _opendata_build_summary_listing($step_1['data_table']);  
	
	//Map field set
	$form['summary_map'] = array(
	    '#type' => 'fieldset',
	    '#title' => t('Map Options'),
	    '#tree' => TRUE,
	);
	$form['summary_map']['map_option'] = array(
	    '#type' => 'checkbox',
	    '#title' => t('Include Map'),
	    '#description' => t('Please check if you would like to show a map.'),
	);
	$form['summary_map']['map_title'] = array(
	    '#type' => 'select',
	    '#title' => t('Title'),
	    '#options' => _opendata_get_fields($step_1['data_table']),
	    '#description' => t('Please choose the column that you would like to use as the title of the map.'),
	);
	$form['summary_map']['map_sub_title'] = array(
	    '#type' => 'select',
	    '#title' => t('Sub Title'),
	    '#options' => _opendata_get_fields($step_1['data_table']),
	    '#description' => t('Please choose the column that you would like to use as the sub title of the map.'),
	);
	$form['summary_map']['map_lat'] = array(
	    '#type' => 'select',
	    '#title' => t('Latitude'),
	    '#options' => _opendata_get_fields($step_1['data_table']),
	    '#description' => t('Please choose the column that holds the latitude value.'),
	);
	$form['summary_map']['map_long'] = array(
	    '#type' => 'select',
	    '#title' => t('Longitude'),
	    '#options' => _opendata_get_fields($step_1['data_table']),
	    '#description' => t('Please choose the column that holds the longitude value.'),
	);
    
	$form['next_detail'] = array(
		'#type' => 'submit',
		'#value' => t('Next'),
	);
	
	return $form;
}

/** 
 * Creates the form that defines the detail page. 
 *
 * @todo write helper functions to guess a label for each field 
 * @todo we also need a function to guess the field for each map option field
 * @todo use a helper function to guess the column names for the summary field
 * @todo combine helper function _opendata_build_summary_listing and 
 *				_opendata_build_detail_listing
 *
 * Process: 
 *	- creates an array called $step_2 to display three items from the first step: 
 *		- grant title 
 *		- data table name 
 *		- fiscal year 
 *
 *	- it calls helper function _opendata_get_tablename($tablename) to get the 
 *		actual table name of the data selected at step_1
 *	- it calls helper function _opendata_build_detail_listing to populate the 
 *		form with the available fields from the data table (passing the name of 
 *		the data table selected at step_1
 *
 *	@param $form_state 
 *		An array holding the form data 
 *	@return 
 *		A $form array with user provided values.
 *
 */
function opendata_detail($form_state) {
    
    //Detail Page
    $step_2 = array();
    
    $step_2['title'] = $form_state['storage']['general_values']['details']['title'];              //Get title of the dataset
    $step_2['data_table'] = $form_state['storage']['general_values']['details']['data_table'];    //Get dataset table
    $step_2['fiscal_year'] = $form_state['storage']['general_values']['details']['fiscal_year'];    //Get dataset table
    
    //Show grant title, data table, fiscal year -- for reference
    $form['r_grant_title'] = array(
        '#type' => 'item',
        '#title' => t('Grant Title - ' . $step_2['title']),
    );
    $form['r_data_table'] = array(
        '#type' => 'item',
        '#title' => t('Data Table - ' . _opendata_get_tablename($step_2['data_table'])),
    );
    $form['r_fiscal_year'] = array(
        '#type' => 'item',
        '#title' => t('Fiscal Year - ' . $step_2['fiscal_year']),
    );
        
    //Append the columns based on the selected data table
    $form[] = _opendata_build_detail_listing($step_2['data_table']);  
    
    $form['detail_map']['map_option'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include Map'),
        '#description' => t('Please check if you would like to show a map.'),
    );
    
	$form['publish'] = array(
		'#type' => 'submit',
		'#value' => t('Publish'),
	);
	
	return $form;
}

/** 
 * Publishes the dataset by creating a view, an OL preset, and a context 
 * using the values submitted by the form.
 *
 * @todo use this function as the control center for putting everything together
 * @todo consider combining this into opendata_general_submit
 *
 * Process: 
 *	- it calls helper function _opendata_get_tablename($tablename) to get the 
 *		actual table name of the data selected at step_1
 *	- it calls helper function _opendata_build_detail_listing to populate the 
 *		form with the available fields from the data table (passing the name of 
 *		the data table selected at step_1
 *
 *	@param $form_state 
 *		An array holding the form data after the last step (detail page info) 
 *	@return 
 *		It returns nothing. It will try to build the various components and 
 *		output a message depending on the outcome.
 *
 */
function opendata_published(&$form_state) {
    
    #debug
    #dpm($form_state);
    
    opendata_general_views($form_state);
}

/**
 * Implementation of hook_form_submit()
 */
function opendata_general_submit($form, &$form_state) {
	if ($form_state['clicked_button']['#id'] == 'edit-next-summary') {
		$form_state['storage']['summary'] = TRUE;    
		$form_state['storage']['general_values'] = $form_state['values'];
		dpm($form_state);
	}
	else if ($form_state['clicked_button']['#id'] == 'edit-next-detail') {
		$form_state['storage']['detail'] = TRUE;    
		$form_state['storage']['summary_values'] = $form_state['values'];
		dpm($form_state);
	}
	else if ($form_state['clicked_button']['#id'] == 'edit-publish') {
		$form_state['storage']['published'] = TRUE;    
		$form_state['storage']['published_values'] = $form_state['values'];
	}
	else {
		$general_values = $form_state['storage']['general_values'];
	}
}

/** 
 *	This used to be an implementation of hook_default_view_views(). 
 * 
 *	It builds the view, OL preset and context based on the user input.
 *
 *	@todo break this function into smaller functions and deprecate it.
 *  @todo consider combining into opendata_general_submit()
 *
 *	Helper functions used:
 *		- _opendata_get_tablename($data_table_id)
 *		- _opendata_formater($office_name)
 *		- _opendata_get_fields($data_table_id)
 *		- _opendata_build_view_summary_fields($table_name, $colnames, $summary_fields)
 *		- _opendata_build_style_options($colnames, $summary_fields)
 *		- opendata_create_preset($preset_name)
 *
 *	@param $form_state 
 *		An array holding the form data after the last step (detail page info) 
 *	@return 
 *		It returns nothing. It will try to build the various components and 
 *		output a message depending on the outcome.
 *
 */
function opendata_general_views(&$form_state) {
    
    #debug
    #dpm($form_state);

    //Basic data points collected from the user
    $data_table_id = $form_state['storage']['general_values']['details']['data_table'];
    $data_table_name = _opendata_get_tablename($data_table_id);
    $office = _opendata_formatter($form_state['storage']['general_values']['details']['office'], 'lower');
    $fiscal_year = $form_state['storage']['general_values']['details']['fiscal_year'];
    $full_title =  $form_state['storage']['general_values']['details']['title'];
    $title =  _opendata_formatter($form_state['storage']['general_values']['details']['title'], 'hyphen');
    $description = _opendata_formatter($form_state['storage']['general_values']['details']['description'], 'html');
    
    $unique_name = substr('od_' . $office . '_' . $fiscal_year . '_' . _opendata_formatter($form_state['storage']['general_values']['details']['title'], 'underscore'), 0, 30);
    
    $page_limit = $form_state['storage']['summary_values']['page_limit'];
    $table_key = $form_state['storage']['summary_values']['table_key'];
    $map_title = $form_state['storage']['summary_values']['summary_map']['map_title'];
    $map_sub_title = $form_state['storage']['summary_values']['summary_map']['map_sub_title'];
    $map_lat = $form_state['storage']['summary_values']['summary_map']['map_lat'];
    $map_long = $form_state['storage']['summary_values']['summary_map']['map_long'];
	
	// check if "Include Map" has been checked for summary and detail pages
	$doMap_summary = $form_state['storage']['summary_values']['summary_map']['map_option'];
	$doMap_detail = $form_state['storage']['detail_map']['map_option'];
	
	//Detail Fields

	//Build a perm URL
    $url = 'grants/' . $office . '/' . $fiscal_year . '/' . $title;
    
    ##
    ## Views
    ##
    
    // set the stage for a new view
    $view = views_new_view();
	
    // define the view (this code was generated by the Export)
    $view->name = $unique_name;
    $view->description = 'View generated by OpenData module for the ' . $fical_year . ' ' . $full_title . ' Dataset';
    $view->tag = $office;
    $view->view_php = '';
    $view->base_table = $data_table_name;
    $view->is_cacheable = FALSE;
    $view->api_version = 2;
    $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
    
    // get the list of column names
    $colnames = array();
    $colnames = _opendata_get_fields($data_table_id);
    
    // get the name of the selected table key field
    $table_key = $colnames[$table_key];
    
    // set the detail perm url based on the selected table key field
    $detail_url = $url . '/detail/[' . $table_key . ']?' . $table_key . '=[' . $table_key . ']';
    
    //Get array of the summary fields
    $summary_fields = array();
    $summary_fields = $form_state['storage']['summary_values']['summary_fields'];
    
	// get an array of the detail page fields
	$detail_fields = array();
    $detail_fields = $form_state['storage']['published_values']['detail_fields'];
    
    ##
    ## Create a 'default' display
    ##
    $handler = $view->new_display('default', $full_title . ' default', 'default');
    
    // build detail page fields
    $selected_fields = array();
    $selected_fields = _opendata_build_view_summary_fields($data_table_name, $colnames, $summary_fields, $detail_url, $table_key);
    $handler->override_option('fields', $selected_fields);
    
    $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'table wizard administration',
    ));
    $handler->override_option('cache', array(
      'type' => 'none',
    ));
    $handler->override_option('header_format', '1');
    $handler->override_option('header_empty', 0);
    $handler->override_option('empty', 'There are no rows in this table.');
    $handler->override_option('empty_format', '1');
    $handler->override_option('items_per_page', $page_limit);
    $handler->override_option('use_pager', '1');
    $handler->override_option('style_plugin', 'table');
    
    // build the style options
    $style_options = array();
    $style_options = _opendata_build_style_options($colnames, $summary_fields);
    $handler->override_option('style_options', $style_options);
    $handler->override_option('row_plugin', '');
    $handler->override_option('displays', array());

    
    ##
    ## Create a 'block' display to show the description text
    ##
    $handler = $view->new_display('block', $full_title . ' description block', 'block_1');
    $handler->override_option('fields', array(
      'nothing' => array(
        'label' => '',
        'alter' => array(
          'text' => $description,
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'exclude' => 0,
        'id' => 'nothing',
        'table' => 'views',
        'field' => 'nothing',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('items_per_page', 1);
    $handler->override_option('use_pager', '0');
    $handler->override_option('style_plugin', 'default');
    $handler->override_option('style_options', NULL);
    $handler->override_option('row_plugin', 'fields');
    $handler->override_option('block_description', '');
    $handler->override_option('block_caching', -1);
    
    
    ##
    ## Create a 'page' display to show table listing
    ##
    $handler = $view->new_display('page', $full_title . ' summary table listing', 'page_1');
    $handler->override_option('title', $full_title);
    $handler->override_option('path', $url);
    
    
    if ($doMap_summary == 1) {
		##
		## Create a 'open layers' display to hold the OL data
		##
		$handler = $view->new_display('openlayers', $full_title . ' map data', 'openlayers_1');
		$handler->override_option('style_plugin', 'openlayers_data');
		$handler->override_option('style_options', array(
			'grouping' => '',
			'data_source' => array(
				'value' => array(
				'other_latlon' => 'other_latlon',
				),
				'other_lat' => $map_lat,
				'other_lon' => $map_long,
				'openlayers_wkt' => $title,
				'other_top' => $title,
				'other_right' => $title,
				'other_bottom' => $title,
				'other_left' => $title,
				'name_field' => $map_title,
				'description_field' => $map_sub_title,
			),
		));
	
	    
		##
		## Create a 'block' display to show the map
		##
	
		// load opendata.preset.inc so we can call the preset function
		module_load_include('inc', 'opendata', 'opendata.preset');
	
		//Create a new OL preset
		$isCreated = opendata_create_preset($unique_name);
	
		//Verify that the OL preset is created successfully
		if ($isCreated) {
		 $handler = $view->new_display('block', $full_title . ' map block', 'block_2');
		 $handler->override_option('items_per_page', 0);
		 $handler->override_option('use_pager', '0');
		 $handler->override_option('style_plugin', 'openlayers_map');
		 $handler->override_option('style_options', array(
			 'preset' => $unique_name,
		 ));
		 $handler->override_option('block_description', '');
		 $handler->override_option('block_caching', -1);
		} else {
		 drupal_set_message(t('An OpenLayer preset fails to be created. Please contact your Administrator.'), 'error');   
		}
	}
    
    
    ##
    ## Create a 'block' display to show the CSV button
    ##
    $handler = $view->new_display('feed', $full_title . ' csv block', 'feed_1');
    $handler->override_option(
        'items_per_page', 0
    );
    $handler->override_option('style_plugin', 'views_csv');
    $handler->override_option('style_options', array(
      'mission_description' => FALSE,
      'description' => '',
      'attach_text' => 'CSV',
      'provide_file' => 1,
      'filename' => $unique_name . '.csv',
      'parent_sort' => 0,
      'seperator' => ',',
      'quote' => 1,
      'header' => 1,
      )
    );
    $handler->override_option('row_plugin', '');
    $handler->override_option('path', $url . '/csv');
    
    $handler->override_option('displays', array(
      'page_1' => 'page_1',
      'default' => 0,
      'block_1' => 0,
    ));
    
    
    ##
    ## Create a 'page' display to show the detail page
    ##

    $handler = $view->new_display('page', $full_title . ' detail', 'page_2');
    
    // build detail page fields
    $selected_fields = array();
    $selected_fields = _opendata_build_detail_view_fields($data_table_name, $colnames, $detail_fields, $table_key);
    $handler->override_option('fields', $selected_fields);
    
    $handler->override_option('arguments', array(
      $table_key => array(
        'id' => $table_key,
        'table' => $data_table_name,
        'field' => $table_key,
        'label' => _opendata_formatter($table_key, 'space'),
        'exclude' => 0,
        'relationship' => 'none',
        'separator' => '',
        'exclude' => 1,
      ),
    ));
    
    $handler->override_option('title', $full_title);
    $handler->override_option('path', $url . '/detail/%');
    $handler->override_option('style_plugin', 'default');
    $handler->override_option('style_options', NULL);
    $handler->override_option('row_plugin', 'fields');

    #debug
    dpm($view);
    
    //Build context
    $cname = $unique_name;
    $tag = 'OpenData Module';
    
    $conditions = array();
    $conditions[] = $unique_name . ':block_1';
    $conditions[] = $unique_name . ':block_2';
    $conditions[] = $unique_name . ':page_1';
    
    $reactions = array();
    $reactions[] = array('views', $unique_name . '-block_1', 'content_top', 0);
    $reactions[] = array('views', $unique_name . '-block_2', 'content_top', 1);
    $reactions[] = array('views', $unique_name . '-page_1', 'content_top', 2);
    
    // load opendata.context.inc so we can call the context function
	module_load_include('inc', 'opendata', 'opendata.context');
	
	// create a new context
    $isCreated = opendata_create_context($cname, $tag, $conditions, $reactions);
    
    if (!$isCreated) {
        drupal_set_message(t('A Context failed to be created. Please contact your Administrator.'), 'error');
    }
    
    
    //Save view
    $view->save();

	// Make sure menu items get rebuilt as neces
	menu_rebuild();

	// Clear the views cache.
	cache_clear_all('*', 'cache_views');

	// Clear the page cache.
	cache_clear_all();

	// Remove this view from cache so we can edit it properly.
	views_object_cache_clear('view', $view->name);
	
	drupal_set_message(t('Dataset Published Successfully!'));
    drupal_set_message(t('Showtime @ http://open.blencorp.com/camel/' . $url));

}

function _opendata_build_view_summary_fields($data_table_name, $colnames, $s_fields, $url, $table_key) {
    
    #debug
    #dpm($s_fields);
    
    // Holds the built out views fields
    $s_selected_fields = array();
    
    // IMPORTANT!
    // set the table key as the first field
    // to pass the right value to the detail page
    $s_selected_fields[$table_key] = array(
        'id' => $table_key,
        'table' => $data_table_name,
        'field' => $table_key,
        'label' => _opendata_formatter($table_key, 'space'),
        'exclude' => 0,
        'relationship' => 'none',
        'separator' => '',
        'exclude' => 1,
    );
    $s_selected_fields[$table_key]['display_order'] = 0;
    

    // find out the table key column name
    $table_key_column = $colnames[$table_key];
    
    // a temporary display order field to hold, if the
    // user does not enter any display order
    $temp_display_order = 999;

    foreach ($colnames as $colname) {    
        // Add the field_ prefix to the column name
        $selected_colname = 'field_selected_' . $colname;
        $sort_colname = 'field_display_order_' . $colname;   
     
        // If the field is selected, build the view field
        if ($s_fields[$selected_colname] == 1) {
                        
            $s_selected_fields[$colname] = array(
                'id' => $colname,
                'table' => $data_table_name,
                'field' => $colname,
                'label' => _opendata_formatter($colname, 'space'),
                'exclude' => 0,
                'relationship' => 'none',
                'alter' => array(
                    'make_link' => 1,
                    'path' => $url,
                ),
            );
            
            //
            if ($s_fields[$sort_colname] != '' or $s_fields[$sort_colname] > 0) {
                $s_selected_fields[$colname]['display_order'] = $s_fields[$sort_colname];
            } else {
                $s_selected_fields[$colname]['display_order'] = $temp_display_order;
                $temp_display_order++;
            }
            
        }
    }
    
    // sort the display order
    $s_selected_fields = _opendata_sort_display_order($s_selected_fields, 1);

    #debug
    #dpm($s_selected_fields);  
    
    return $s_selected_fields;
}

function _opendata_build_detail_view_fields($data_table_name, $colnames, $d_fields, $table_key) {
    
    #debug
    #dpm($d_fields);
    
    // Holds the built out views fields
    $d_selected_fields = array();
    
    // IMPORTANT!
    // set the table key as the first field
    // to pass the right value to the detail page
    $d_selected_fields[$table_key] = array(
        'id' => $table_key,
        'table' => $data_table_name,
        'field' => $table_key,
        'label' => _opendata_formatter($table_key, 'space'),
        'exclude' => 0,
        'relationship' => 'none',
        'separator' => '',
        'exclude' => 1,
    );
    $d_selected_fields[$table_key]['display_order'] = 0;
    

    // find out the table key column name
    $table_key_column = $colnames[$table_key];
    
    // a temporary display order field to hold, if the
    // user does not enter any display order
    $temp_display_order = 999;

    foreach ($colnames as $colname) {    
        // Add the field_ prefix to the column name
        $selected_colname = 'field_selected_' . $colname;
        $sort_colname = 'field_display_order_' . $colname;   
     
        // If the field is selected, build the view field
        if ($d_fields[$selected_colname] == 1) {
            
            $d_selected_fields[$colname] = array(
                'id' => $colname,
                'table' => $data_table_name,
                'field' => $colname,
                'label' => '',
                'exclude' => 0,
                'relationship' => 'none',
                'alter' => array(
                    'alter_text' => 1,
					'text' => '<div class="detail"><strong>' . _opendata_formatter($colname, 'space') . '</strong><p>[' . $colname . ']</p></div>',
                ),
            );
            
            //
            if ($d_fields[$sort_colname] != '' or $d_fields[$sort_colname] > 0) {
                $d_selected_fields[$colname]['display_order'] = $d_fields[$sort_colname];
            } else {
                $d_selected_fields[$colname]['display_order'] = $temp_display_order;
                $temp_display_order++;
            }
            
        }
    }
    
    // sort the display order
    $d_selected_fields = _opendata_sort_display_order($d_selected_fields);

    #debug
    #dpm($d_selected_fields);  
    
    return $d_selected_fields;
}

function _opendata_sort_display_order($selected_fields, $isSummary = 0) {

    foreach ($selected_fields as $key => $row) {
        $selected_field[$key] = $row['display_order'];
    }
    
    array_multisort($selected_field, SORT_ASC, $selected_fields);
    
	// remove path for only summary fields
	if ($isSummary) {
		// hold index of the array
		$i = 0;
		
		foreach ($selected_fields as $key => $value) {        
			
			if ($i > 1) {
				// delete the 'path' array item				
				//unset($selected_field['alter']['path']);
				unset($selected_fields[$key]['alter']['path']);
			}
			

			// increment the current index
			$i++;
		}
	}
    #dpm($selected_fields);
    
    return $selected_fields;
}

function _opendata_build_style_options($colnames, $fields) {

    $style_options = array();
    $style_options_columns = array();
    $style_options_info = array();
    
    foreach ($colnames as $colname) {    
        //Add the field_ prefix to the column name
        $selected_colname = 'field_selected_' . $colname;
        
        //If the field is selected, build the view field
        if ($fields[$selected_colname] == 1) {
            
            //Add it to the columns
            $style_options_columns[$colname] = $colname;
                        
            //Add it to the info

						// check to see if the field is made sortable 
						// then set style accordingly
						$fld_sortable = 'field_sortable_' . $colname;
						$is_sortable = $fields[$fld_sortable];
						if ($is_sortable) {
							$style_options_info[$colname] = array ('sortable' => 1, 'separator' => '',);
						} else {
							$style_options_info[$colname] = array ('sortable' => 0, 'separator' => '',);
						}
        }
    }

		//dpm($fields);
    
    $style_options = array (
      'grouping' => '',
      'override' => 1,
      'sticky' => 1,
      'order' => 'asc',
      'columns' => $style_options_columns,
      'info' => $style_options_info,
      'default' => -1,
    );
    
    #Debug
    #dpm($style_options);
    
    return $style_options;
    
    /* View Template: Reference Only */
    /*
          'grouping' => '',
      'override' => 1,
      'sticky' => 1,
      'order' => 'asc',
      'columns' => array(
        'ID' => 'ID',
        'awardNo' => 'awardNo',
        'projectTitle' => 'projectTitle',
        'projectDirector' => 'projectDirector',
        'amount' => 'amount',
        'teacherNo' => 'teacherNo',
        'districtNo' => 'districtNo',
        'grades' => 'grades',
      ),
      
      'info' => array(
        'ID' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'awardNo' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'projectTitle' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'projectDirector' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'amount' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'teacherNo' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'districtNo' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'grades' => array(
          'sortable' => 1,
          'separator' => '',
        ),
      ),
      'default' => -1,
    ));

    */

}


/**
 * A helper function to build the fields of the columns
 * 
 * @param
 *   Selected table ID
 *
 * @return 
 *   A form array
 */
function _opendata_build_summary_listing($tableid) {
	//Debug
	//dpm($tableid);
	
	$form['summary_fields'] = array(
		'#type' => 'fieldset',
		'#title' => t('Fields to Include on Summary Listings'),
		'#tree' => TRUE,
		'#prefix' => '<div class="opendata-summary-fields">',
		'#suffix' => '</div>',
	);
	
	$form['summary_fields']['lbl_display_fields'] = array(
	    '#type' => 'item',
	    '#title' => t('Summary Listing Fields'),
	);
	$form['summary_fields']['lbl_display_order'] = array(
	    '#type' => 'item',
	    '#title' => t('Display Field'),
	);
	$form['summary_fields']['lbl_allow_search'] = array(
	    '#type' => 'item',
	    '#title' => t('Allow Search?'),
	);
	$form['summary_fields']['lbl_sortable'] = array(
	    '#type' => 'item',
	    '#title' => t('Sortable?'),
	);
	
	//Build the columns
	$colnames = array();
	$colnames = _opendata_get_fields($tableid);
	
	// Build the fields
	$fields = array();
	
	foreach ($colnames as $colname) {
	  $fields[$colname] = array(
	    'id' => $colname,
	    'table' => $rawtablename,
	    'field' => $colname,
	    'label' => $colname,
	    'exclude' => 0,
	    'relationship' => 'none',
	  );
	  
	  //Generate the table fields checkboxes
	  $selected = 'field_selected_' . $colname;
	  $form['summary_fields'][$selected] = array(
	    '#type' => 'checkbox', 
	    '#title' => t($colname)
	  );
	  
	  //Generate Sort order fields
	  $display_order = 'field_display_order_' . $colname;
	  $form['summary_fields'][$display_order] = array(
			'#type' => 'textfield', 
			'#size' => 10
		);
	  
	  //Generate filter options
	  $filter_options = 'field_filter_' . $colname;
	  $form['summary_fields'][$filter_options] = array(
	    '#type' => 'select',
	    '#options' => array(
	        'na'=>'',
	        'textfield'=>'Text Search',
	        'select'=>'Dropdown',
	        'checkbox'=>'Checkboxes',
	    ),
	  );
	  
	  //Generate the sortable checkboxes
	  $sortable = 'field_sortable_' . $colname;
	  $form['summary_fields'][$sortable] = array('#type' => 'checkbox', '#label' => 'Sortable');
	}
	
	//Debug
	//dpm($form);
	
	return $form;
}

/**
 * A helper function to build the fields of the columns
 * 
 * @param
 *   Selected table ID
 *
 * @return 
 *   A form array
 */
function _opendata_build_detail_listing($tableid) {
    
    //Debug
    //dpm($tableid);
    
    $form['detail_fields'] = array(
        '#type' => 'fieldset',
        '#title' => t('Fields to Include on Detail Page'),
        '#tree' => TRUE,
				'#prefix' => '<div class="opendata-detail-fields">',
				'#suffix' => '</div>',
    );
    
    //Build the columns
    $colnames = array();
    $colnames = _opendata_get_fields($tableid);
    
    $form['detail_fields']['lbl_display_fields'] = array(
	    '#type' => 'item',
	    '#title' => t('Detail Listing Fields'),
	);
	$form['detail_fields']['lbl_display_order'] = array(
	    '#type' => 'item',
	    '#title' => t('Display Field'),
	);
	$form['detail_fields']['lbl_title'] = array(
	    '#type' => 'item',
	    '#title' => t('Title'),
	);
	$form['detail_fields']['lbl_line_break'] = array(
	    '#type' => 'item',
	    '#title' => t('Line Break'),
	);
	$form['detail_fields']['lbl_badge'] = array(
	    '#type' => 'item',
	    '#title' => t('Badge'),
	);
    
    // Build the fields
    $fields = array();
    
    foreach ($colnames as $colname) {
      $fields[$colname] = array(
        'id' => $colname,
        'table' => $rawtablename,
        'field' => $colname,
        'label' => $colname,
        'exclude' => 0,
        'relationship' => 'none',
      );
      
      //Generate the table fields checkboxes
      $selected = 'field_selected_' . $colname;
      $form['detail_fields'][$selected] = array('#type' => 'checkbox', '#title' => t($colname));
      
      //Generate Sort order fields
      $display_order = 'field_display_order_' . $colname;
      $form['detail_fields'][$display_order] = array('#type' => 'textfield', '#size' => 10);
      
      // generate title option
      $detail_title = 'field_detail_title_' . $colname;
      $form['detail_fields'][$detail_title] = array(
        '#type' => 'radios',
        '#options' => array(
            '0' => ''
        ),
      );
      
      // generate the line break
      $line_break = 'field_line_break_' . $colname;
      $form['detail_fields'][$line_break] = array('#type' => 'checkbox');
      
      // generate the badge field
      $badge = 'field_badge_' . $colname;
      $form['detail_fields'][$badge] = array('#type' => 'checkbox');
    }
    
    //dpm($form);
    
    return $form;
}

/**
 * A helper function for getting the columns
 * 
 * @param
 *   Table name
 *
 * @return 
 *   Column names 
 */
function _opendata_get_fields($table_name) {

    $pk = NULL;
    $colnames = array();
    $sql = "SELECT colname, primarykey
            FROM {tw_columns}
            WHERE ignorecol=0 AND twtid=%d
            ORDER BY weight";
    $colresult = db_query($sql, $table_name);
              
    while ($colrow = db_fetch_object($colresult)) {
      $colnames[] = $colrow->colname;
      if ($colrow->primarykey) {
        $pk = $colrow->colname;
      }
    }
    
    #debug
    #dpm($colnames);
    
    return $colnames;
}

/**
 * A helper function for getting the table names
 * 
 * @param
 *
 * @return 
 *   Array of table names
 */
function _opendata_get_tables() {
    
    //Set array to hold the table names
    $tablenames = array();

    // Go through each import table and create default views for them
    // Skip any without a primary key
     $sql = "SELECT tt.twtid,tt.tablename,tt.dbconnection,tt.view_name,COUNT(*) AS cnt
          FROM {tw_tables} tt
          INNER JOIN {tw_columns} tc ON tt.twtid=tc.twtid
          WHERE tt.provide_view=1 AND tc.primarykey=1
          GROUP BY tt.twtid,tt.tablename,tt.view_name,tt.dbconnection";
    $tblresult = db_query($sql);

    while ($tblrow = db_fetch_object($tblresult)) {
        // For a default view to be possible, the table must have a single primary key field
        if ($tblrow->cnt != 1) {
          continue;
        }
        
        $twtid = $tblrow->twtid;
        $tablename = $tblrow->tablename;
        $view_name = $tblrow->view_name;
        $dbconnection = $tblrow->dbconnection;
        if ($dbconnection == 'default') {
          $rawtablename = schema_unprefix_table($tablename);
          $cleantablename = $rawtablename;
        }
        else {
          $truedbname = tw_get_dbinfo($dbconnection, 'name');
          $rawtablename = tw_qualified_tablename($dbconnection, $tablename);
          $cleantablename = $truedbname . '_' . $tablename;
        }
        
        $tablenames[$twtid] = $tablename;
    }
    
    #Debug
    #dpm($tablenames);
    
    return $tablenames;
}

/**
 * A helper function for getting the table names
 * 
 * @param
 *   Selected table ID
 *
 * @return 
 *   Selected table name
 */
function _opendata_get_tablename($tableid) {

    //Get table name
    $data_table_names = array();
    $data_table_names = _opendata_get_tables();
    $data_table_name = $data_table_names[$tableid];
    
    return $data_table_name;
}

/**
 * A helper function for formatting strings to desired type
 * 
 * @param
 *   The preset title provided by the user
 *
 * @return 
 *   A formatted text string
 */
function _opendata_formatter($val, $type) {
    switch ($type) {
        case 'lower':
            $val = strtolower($val);
            break;
        case 'hyphen':
            $val = str_replace(" ", "-", strtolower($val));
            break;
        case 'html':
            nl2br($val);
            break;
        case 'underscore':
            $val = str_replace(" ", "_", strtolower($val));
            break;
        case 'space':
            $val = str_replace("_", " ", ucwords($val));
            break;
    }
    
    return $val;
}

/*
function opendata_general_view1(&$form_state) {
    $fields_data_table  = strtolower($form_state['values']['data_table']);
	$fields_office = strtolower($form_state['values']['office']);
	$fields_title = str_replace(" ", "-", strtolower($form_state['values']['title']));
	$fields_description = nl2br($form_state['values']['description']);
	$fields_more_info_url = $form_state['values']['more_info_url'];
	
    
    $view = views_get_view('name_of_view');
    $view->set_display($display_id);
    $filter = $view->get_item($display_id, 'filter', 'name_of_filter');
    $filter['value']['value'] = $filter_value;
    $view->set_item($display_id, 'filter', 'name_of_filter', $filter);
}

function opendata_generate_view2(&$form_state) {
  $views = array();
 
  // Go through each import table and create default views for them
  // Skip any without a primary key
  $sql = "SELECT tt.twtid,tt.tablename,tt.dbconnection,tt.view_name,COUNT(*) AS cnt
          FROM {tw_tables} tt
          INNER JOIN {tw_columns} tc ON tt.twtid=tc.twtid
          WHERE tt.provide_view=1 AND tc.primarykey=1
          GROUP BY tt.twtid,tt.tablename,tt.view_name,tt.dbconnection";
  $tblresult = db_query($sql);
  while ($tblrow = db_fetch_object($tblresult)) {
    // For a default view to be possible, the table must have a single primary key field
    if ($tblrow->cnt != 1) {
      continue;
    }

    $twtid = $tblrow->twtid;
    $tablename = $tblrow->tablename;
    $view_name = $tblrow->view_name;
    $dbconnection = $tblrow->dbconnection;
    if ($dbconnection == 'default') {
      $rawtablename = schema_unprefix_table($tablename);
      $cleantablename = $rawtablename;
    }
    else {
      $truedbname = tw_get_dbinfo($dbconnection, 'name');
      $rawtablename = tw_qualified_tablename($dbconnection, $tablename);
      $cleantablename = $truedbname . '_' . $tablename;
    }
    // Create a basic table view, with exclusion flags, for each import table
    $view = new view;
    $view->name = $view_name;
    $view->description = $tablename;
    $view->tag = 'tw';
    $view->view_php = '';
    $view->base_table = $rawtablename;
    $view->is_cacheable = FALSE;
    $view->api_version = 2;
    $view->disabled = FALSE;
    $handler = $view->new_display('default', $view_name, 'default');

    // Grab the available columns for this table (skip any marked ignore)
    $pk = NULL;
    $colnames = array();
    $sql = "SELECT colname, primarykey
            FROM {tw_columns}
            WHERE ignorecol=0 AND twtid=%d
            ORDER BY weight";
    $colresult = db_query($sql, $twtid);
    while ($colrow = db_fetch_object($colresult)) {
      $colnames[] = $colrow->colname;
      if ($colrow->primarykey) {
        $pk = $colrow->colname;
      }
    }

    // Add our columns to the fields
    $fields = array();
    foreach ($colnames as $colname) {
      $fields[$colname] = array(
        'id' => $colname,
        'table' => $rawtablename,
        'field' => $colname,
        'label' => $colname,
        'exclude' => 0,
        'relationship' => 'none',
      );
    }

    // Add columns for joined tables
    $sql = "SELECT twc2.colname, twt2.tablename
            FROM {tw_tables} twt1
            INNER JOIN {tw_columns} twc1 ON twt1.twtid=twc1.twtid
            INNER JOIN {tw_relationships} twr ON twc1.twcid=twr.leftcol AND twr.automatic=1
            INNER JOIN {tw_columns} twc2 ON twr.rightcol=twc2.twcid
            INNER JOIN {tw_tables} twt2 ON twc2.twtid=twt2.twtid
            WHERE twt1.twtid=%d";
    $result = db_query($sql, $twtid);
    while ($row = db_fetch_object($result)) {
      $colname = $row->colname;
      $colnames[] = $colname;
      $fields[$colname] = array(
        'id' => $colname,
        'table' => $row->tablename,
        'field' => $colname,
        'label' => $colname,
        'exclude' => 0,
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'strip_tags' => 0,
          'html' => 0,
        ),
      );
    }

    $handler->override_option('fields', $fields);
    $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => TW_ACCESS,
    ));
    $handler->override_option('title', t('Contents of !tablename', array('!tablename' => $rawtablename)));
    $handler->override_option('header', t('This is a view of a raw database table. It may be ' .
      'sorted in various ways by clicking the column headers.

      If you identify a particular field that does not need to be used in views of this table, ' .
      'go to the <a href="!analyze_url">analysis page</a> ' .
      'and check the <em>Ignore</em> box for that field. It will then no longer appear here.',
      array('!analyze_url' => url("admin/content/tw/analyze/$twtid"))));
    $handler->override_option('header_format', '1');
    $handler->override_option('header_empty', 0);
    $handler->override_option('empty', 'There are no rows in this table.');
    $handler->override_option('empty_format', '1');
    $handler->override_option('items_per_page', 25);
    $handler->override_option('use_pager', '1');
    $handler->override_option('style_plugin', 'table');
    // Add our columns into the style options
    $columns = array();
    foreach ($colnames as $colname) {
      $columns[$colname] = $colname;
    }
    $info = array();
    foreach ($colnames as $colname) {
      $info[$colname] = array(
        'sortable' => 1,
        'separator' => '',
      );
    }
    $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 1,
      'order' => 'asc',
      'columns' => $columns,
      'info' => $info,
      'default' => -1,
    ));
    $handler = $view->new_display('page', 'Page', 'page_1');
    $handler->override_option('path', "admin/content/tw/view/$view_name");
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'weight' => 0,
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'weight' => 0,
    ));

    $views[$view->name] = $view;
  }
  
  dpm($views);
  
  return $views;
}


function opendata_generate_view1(&$form_state) {

	//Set the path of the view
	$_output_path = 'temp/grants/' . $fields_office . '/2011/' . $fields_title;

    //create a new view
    $view = views_new_view();
    
    //define the view (this code was generated by the Export)
    $view->name = 'temp_' . $fields_title; //'rtl_listing';
    $view->description = 'Testing View';
    $view->tag = '';
    $view->view_php = '';
    $view->base_table = 'node';
    $view->is_cacheable = FALSE;
    $view->api_version = 2;
    $view->disabled = FALSE;
    
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->override_option('fields', array(
      'nid' => array(
        'label' => 'Nid',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'exclude' => 1,
        'id' => 'nid',
        'table' => 'node',
        'field' => 'nid',
        'relationship' => 'none',
      ),
      'field_rtl_funding_status_value' => array(
        'label' => 'Funding Status',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 0,
        'id' => 'field_rtl_funding_status_value',
        'table' => 'node_data_field_rtl_funding_status',
        'field' => 'field_rtl_funding_status_value',
        'relationship' => 'none',
      ),
      'field_rtl_organization_value' => array(
        'label' => 'Applicant',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 1,
          'path' => 'temp/ready-to-learn/applicant/[nid]?ID=[nid]',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'custom',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 0,
        'id' => 'field_rtl_organization_value',
        'table' => 'node_data_field_rtl_organization',
        'field' => 'field_rtl_organization_value',
        'relationship' => 'none',
      ),
      'title' => array(
        'label' => 'Project Title',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 1,
          'path' => 'temp/ready-to-learn/applicant/[nid]?ID=[nid]',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'exclude' => 0,
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
      ),
      'field_rtl_city_value' => array(
        'label' => 'City',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 1,
        'id' => 'field_rtl_city_value',
        'table' => 'node_data_field_rtl_city',
        'field' => 'field_rtl_city_value',
        'relationship' => 'none',
      ),
      'field_rtl_state_value' => array(
        'label' => 'Application Location',
        'alter' => array(
          'alter_text' => 1,
          'text' => '[field_rtl_city_value], [field_rtl_state_value] ',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'custom',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 0,
        'id' => 'field_rtl_state_value',
        'table' => 'node_data_field_rtl_state',
        'field' => 'field_rtl_state_value',
        'relationship' => 'none',
      ),
      'field_rtl_subject_value' => array(
        'label' => 'Subject',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 0,
        'id' => 'field_rtl_subject_value',
        'table' => 'node_data_field_rtl_subject',
        'field' => 'field_rtl_subject_value',
        'relationship' => 'none',
      ),
      'field_rtl_platform_value' => array(
        'label' => 'Platform',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 1,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 0,
        'id' => 'field_rtl_platform_value',
        'table' => 'node_data_field_rtl_platform',
        'field' => 'field_rtl_platform_value',
        'relationship' => 'none',
      ),
      'field_rtl_latitude_value' => array(
        'label' => 'Latitude',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 1,
        'id' => 'field_rtl_latitude_value',
        'table' => 'node_data_field_rtl_latitude',
        'field' => 'field_rtl_latitude_value',
        'relationship' => 'none',
      ),
      'field_rtl_longitude_value' => array(
        'label' => 'Longitude',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'label_type' => 'widget',
        'format' => 'default',
        'multiple' => array(
          'group' => TRUE,
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_reversed' => FALSE,
        ),
        'exclude' => 1,
        'id' => 'field_rtl_longitude_value',
        'table' => 'node_data_field_rtl_longitude',
        'field' => 'field_rtl_longitude_value',
        'relationship' => 'none',
      ),
      'nothing' => array(
        'label' => 'City State Status',
        'alter' => array(
          'text' => '[field_rtl_state_value]<br/>
    [field_rtl_funding_status_value]',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'exclude' => 1,
        'id' => 'nothing',
        'table' => 'views',
        'field' => 'nothing',
        'override' => array(
          'button' => 'Override',
        ),
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('filters', array(
      'type' => array(
        'operator' => 'in',
        'value' => array(
          'rtl' => 'rtl',
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
      ),
    ));
    $handler->override_option('access', array(
      'type' => 'none',
    ));
    $handler->override_option('cache', array(
      'type' => 'none',
    ));
    $handler->override_option('items_per_page', 0);
    $handler->override_option('use_pager', '1');
    $handler->override_option('style_plugin', 'table');
    $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'desc',
      'columns' => array(
        'nid' => 'nid',
        'field_rtl_funding_status_value' => 'field_rtl_funding_status_value',
        'field_rtl_organization_value' => 'field_rtl_organization_value',
        'title' => 'title',
        'field_rtl_city_value' => 'field_rtl_city_value',
        'field_rtl_state_value' => 'field_rtl_state_value',
        'field_rtl_subject_value' => 'field_rtl_subject_value',
        'field_rtl_platform_value' => 'field_rtl_platform_value',
      ),
      'info' => array(
        'nid' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_funding_status_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_rtl_organization_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'title' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_rtl_city_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_state_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_subject_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_rtl_platform_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
      ),
      'default' => 'field_rtl_funding_status_value',
    ));
    $handler = $view->new_display('page', 'RTL Listing Page', 'page_1');
    $handler->override_option('sorts', array(
      'field_display_order_order_value' => array(
        'order' => 'DESC',
        'delta' => -1,
        'id' => 'field_display_order_order_value',
        'table' => 'node_data_field_display_order_order',
        'field' => 'field_display_order_order_value',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ),
      'field_rtl_organization_value' => array(
        'order' => 'ASC',
        'delta' => -1,
        'id' => 'field_rtl_organization_value',
        'table' => 'node_data_field_rtl_organization',
        'field' => 'field_rtl_organization_value',
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
      ), ));
    $handler->override_option('title', 'Ready to Learn Applicants');
    $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'desc',
      'columns' => array(
        'nid' => 'nid',
        'field_rtl_funding_status_value' => 'field_rtl_funding_status_value',
        'field_rtl_organization_value' => 'field_rtl_organization_value',
        'title' => 'title',
        'field_rtl_city_value' => 'field_rtl_city_value',
        'field_rtl_state_value' => 'field_rtl_state_value',
        'field_rtl_subject_value' => 'field_rtl_subject_value',
        'field_rtl_platform_value' => 'field_rtl_platform_value',
        'field_rtl_latitude_value' => 'field_rtl_latitude_value',
        'field_rtl_longitude_value' => 'field_rtl_longitude_value',
      ),
      'info' => array(
        'nid' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_funding_status_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_rtl_organization_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'title' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_rtl_city_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_state_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_subject_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_platform_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_latitude_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_rtl_longitude_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
      ),
      'default' => '-1',
    ));
    $handler->override_option('path', $_output_path);
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'details' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'details' => 'navigation',
    ));

    $handler->override_option('sitename_title', FALSE);
    $handler = $view->new_display('openlayers', 'RTL OpenLayers Data', 'openlayers_1');
    $handler->override_option('style_plugin', 'openlayers_data');
    $handler->override_option('style_options', array(
      'grouping' => 'field_rtl_state_value',
      'data_source' => array(
        'value' => array(
          'other_latlon' => 'other_latlon',
        ),
        'other_lat' => 'field_rtl_latitude_value',
        'other_lon' => 'field_rtl_longitude_value',
        'openlayers_wkt' => 'nid',
        'other_top' => 'nid',
        'other_right' => 'nid',
        'other_bottom' => 'nid',
        'other_left' => 'nid',
        'name_field' => 'title',
        'description_field' => 'nothing',
      ),
    ));
    $handler->override_option('row_plugin', '');
    $handler->override_option('displays', array());
    $handler = $view->new_display('block', 'RTL Map Block', 'block_2');
    $handler->override_option('style_plugin', 'openlayers_map');
    $handler->override_option('style_options', array(
      'preset' => 'ready_to_learn',
    ));
    $handler->override_option('block_description', '');
    $handler->override_option('block_caching', -1);

    
    // now output the view (or whatever you want to do with it)
    //print "Output"; 
    //print $view->execute_display('default', array());

    //Save view
    $view->save();
    
    //print 'Page Published @ http://open.blencorp.com/camel/temp/ready-to-learn';
	
	//drupal_set_message(t('Page Published @ http://open.blencorp.com/camel/' . $_output_path));
	drupal_set_message(t('Page Published @ ' . $base_path() . $_output_path));
}

function opendata_display_csv() {
     //CSV Output
    $handler = $view->new_display('feed', 'RTL CSV', 'feed_2');
    $handler->override_option('style_plugin', 'views_csv');
    $handler->override_option('style_options', array(
      'mission_description' => FALSE,
      'description' => '',
      'attach_text' => 'CSV',
      'provide_file' => 1,
      'filename' => 'ready_to_learn.csv',
      'parent_sort' => 0,
      'seperator' => ',',
      'quote' => 1,
      'header' => 1,
    ));
    $handler->override_option('row_plugin', '');
    $handler->override_option('path', 'temp/ready-to-learn/csv');
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'details' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'details' => 'navigation',
    ));
    $handler->override_option('displays', array(
      'page_1' => 'page_1',
      'default' => 0,
      'block_1' => 0,
    ));
    
    return $handler;
}
*/

