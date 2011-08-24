<?php
	function _test_create_view() {
		
		//$view = views_get_view('test_simple_view');
		//$view->delete();
		//drupal_set_message(t('Deleted existing test_simple_view.'), 'status');

		$view = views_new_view();
		$view->name = 'test_simple_view';
		$view->description = '';
		$view->tag = '';
		$view->view_php = '';
		$view->base_table = 'ed_sig';
		$view->is_cacheable = FALSE;
		$view->api_version = 2;
		$view->disabled = FALSE; // Edit this to true to make a default view disabled initially 

		$handler = $view->new_display('default', 'Defaults', 'default');
		$handler->override_option('fields', array(
		'ID' => array(
		'label' => 'ID',
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
		'set_precision' => FALSE,
		'precision' => 0,
		'decimal' => '.',
		'separator' => ',',
		'prefix' => '',
		'suffix' => '',
		'exclude' => 0,
		'id' => 'ID',
		'table' => 'ed_main',
		'field' => 'ID',
		'relationship' => 'none',
		),
		'LEAId' => array(
		'label' => 'LEAId',
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
		'set_precision' => FALSE,
		'precision' => 0,
		'decimal' => '.',
		'separator' => ',',
		'prefix' => '',
		'suffix' => '',
		'exclude' => 0,
		'id' => 'LEAId',
		'table' => 'ed_main',
		'field' => 'LEAId',
		'relationship' => 'none',
		),
		'SCid' => array(
		'label' => 'SCid',
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
		'set_precision' => FALSE,
		'precision' => 0,
		'decimal' => '.',
		'separator' => ',',
		'prefix' => '',
		'suffix' => '',
		'exclude' => 0,
		'id' => 'SCid',
		'table' => 'ed_main',
		'field' => 'SCid',
		'relationship' => 'none',
		),
		));
		$handler->override_option('access', array(
		'type' => 'none',
		));
		$handler->override_option('cache', array(
		'type' => 'none',
		));
		$handler = $view->new_display('page', 'Page', 'page_1');
		$handler->override_option('path', 'simpleview');
		$handler->override_option('menu', array(
		'type' => 'none',
		'title' => '',
		'description' => '',
		'weight' => 0,
		'name' => 'navigation',
		));
		$handler->override_option('tab_options', array(
		'type' => 'none',
		'title' => '',
		'description' => '',
		'weight' => 0,
		'name' => 'navigation',
		));

		$view->save();

		// Make sure menu items get rebuilt as neces
		menu_rebuild();

		// Clear the views cache.
		cache_clear_all('*', 'cache_views');

		// Clear the page cache.
		cache_clear_all();

		// Remove this view from cache so we can edit it properly.
		views_object_cache_clear('view', $view->name);
	}
