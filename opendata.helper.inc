<?php
/**
 * @file
 * OpenData Module Helper Functions
 * This file is not currently being included by any procedures.
 */

function _opendata_build_map_fields($data_table_name, $lat, $lon, $title, $subtitle) {
	// instantiate an array to hold the final fields structure 
	$map_fields = array();

	$map_fields[$lat] = array(
		'id' => $lat,
		'table' => $data_table_name,
		'field' => $lat,
		'label' => _opendata_formatter($lat, 'space'),
		'relationship' => 'none',
		'separator' => '',
		'exclude' => 1,
	);

	$map_fields[$lon] = array(
		'id' => $lon,
		'table' => $data_table_name,
		'field' => $lon,
		'label' => _opendata_formatter($lon, 'space'),
		'relationship' => 'none',
		'separator' => '',
		'exclude' => 1,
	);

	$map_fields[$title] = array(
		'id' => $title,
		'table' => $data_table_name,
		'field' => $title,
		'label' => _opendata_formatter($title, 'space'),
		'relationship' => 'none',
		'separator' => '',
		'exclude' => 1,
	);

	$map_fields[$subtitle] = array(
		'id' => $subtitle,
		'table' => $data_table_name,
		'field' => $subtitle,
		'label' => _opendata_formatter($subtitle, 'space'),
		'exclude' => 0,
		'relationship' => 'none',
		'separator' => '',
	);
				
	return $map_fields;
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
            $style_options_info[$colname] = array ('sortable' => 1, 'separator' => '',);
        }
    }
    
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
