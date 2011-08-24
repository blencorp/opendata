<?php
/** 
 *	It creates a context based on input 
 * 
 *	@param $cname 
 *		The name of the context 
 *		Remember that you will need to be able to delete this on command. 
 *		To delete, call context_delete($context) and the delete function 
 *		will extract the name and delete the object from DB.
 *
 *	@param $tag
 *		A tag for the context. This should be the name of the dataset. 
 *		If necessary, get &$form_state and assign one automatically. 
 *
 *	@param $conditions 
 *		An array of conditions for the context. A condition is a string containing view name	
 *
 *	@param $reactions 
 *		An array of reactions for the context. A reaction is an array of four strings
 *		Ex: $reactions[] = array("module", "name", "region", "weight");
 *		"region" can be "content_top, "right", "footer", or "header"
 *		"weight" can be 1, 2, 3, etc. 
 *
 *	@return 
 *		A boolean on whether or not the context was created.
 *
 */

function opendata_create_context($cname, $tag, $conditions, $reactions) {
	$context = new stdClass;
	$context->disabled = FALSE; // Edit this to true to make a default context disabled initially 
	$context->api_version = 3;
	$context->name = $cname;
	$context->description = 'A context created by OpenData module.';
	$context->tag = $tag;

	// remember that the context function will be called twice, 
	// once for listing and another for detail

	// build views value array
	$vv = array();
	foreach($conditions as &$val) {
		//$vv = array($val => $val);
		$vv[$val] = $val;
	}

	// the only condition we will pay attention to at this time is of type views 
	$context->conditions = array(
		'views' => array(
			'values' => $vv,
		),
	);

	/*
	$context->conditions = array(
		'views' => array(
			'values' => array(
				'od_oii_2013_school_improvement' => 'od_oii_2013_school_improvement',
			),
		),
	);
	*/


	// build reaction block array
	// format: $reactions[] = array("module", "name", "region", "weight");
	$blocks = array();
	foreach($reactions as &$val) {
		$blocks[$val[0] . '-' . $val[1]] = array('module' => $val[0], 'delta' => $val[1], 'region' => $val[2], 'weight' => $val[3]);
	}

	/*
	$context->reactions = array(
		'block' => array(
			'blocks' => array(
				'views-66d78f6cfc0e925fd18e4fd73e00289b' => array(
					'module' => 'views',
					'delta' => '66d78f6cfc0e925fd18e4fd73e00289b',
					'region' => 'content_top',
					'weight' => 0,
				),
				'views-1f19bde15246c669f1af763c8a562913' => array(
					'module' => 'views',
					'delta' => '1f19bde15246c669f1af763c8a562913',
					'region' => 'content_top',
					'weight' => 1,
				),
			), 
		),
	);
	*/

	$context->reactions = array(
		'block' => array(
			'blocks' => $blocks,
		),
	);

	if (context_save($context)) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/** 
 * This is an implementation of hook_form to test the context. 
 */
function opendata_context_form($form_state) {
	$form = array();

	$form['context_title'] = array (
		'#type' => 'textfield', 
		'#title' => 'context title', 
		'#default_value' => '', 
		'#description' => 'A title for the context you want to create',
	);

	$form['context_tag'] = array (
		'#type' => 'textfield', 
		'#title' => 'context tag', 
		'#default_value' => '', 
		'#default_value' => 'TEST',
		'#description' => 'A tag for the context',
	);

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => 'Submit',
	);

	$form['#validate'][] = 'opendata_context_form_validate';

	return $form;
}

/** 
 * This is an implementation of hook_form_validate to test the context.
 */
function opendata_context_form_validate($form, &$form_state) {
	if ($form_state['values']['context_title'] == '') {
		form_set_error('context_title', t('You must provide a title for the context.'));
	}
}

/**
 * This is an implementation of hook_form_submit to test the context. 
 */
function opendata_context_form_submit($form, &$form_state) {
	$cname = $form_state['values']['context_title'];
	$tag = $form_state['values']['context_tag'];

	$conditions[] = "school_improvement_grants_filter:page3";

	$reactions[] = array("block", "6", "content_top", "0");
	$reactions[] = array("boxes", "welcome", "content_top", "1");
	$reactions[] = array("views", "ed_cag_schools-block_1", "content_top", "2");

	if (opendata_create_context($cname, $tag, $conditions, $reactions)) {
		drupal_set_message(t('Success: context ' . $cname . ' has been created.'));
	} else {
		drupal_set_message(t('Error: could not create context.'));
	}
}

