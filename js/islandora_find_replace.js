/**
 * @file
 * check if max_input_vars is met on form.
 */

const maxInputVars = Drupal.settings.islandora_find_replace.maxInputVars;
const message = `You have selected more checkboxes than this system can handle (max_input_vars: ${maxInputVars}).`;
const markup = `<div id="console" class="clearfix"><div id="find_replace_warning" class="messages error"><h2 class="element-invisible">Status message</h2><pre>${message}</pre></div></div>`;

(function ($) {
  Drupal.behaviors.tooManyMessage = {
    attach: function (context, settings) {

      $.fn.tooManySelected = function () {
        const selected = this.parent().parent().parent('.selected').length;
        const messageExists = $('#find_replace_warning').length;

        if (selected < maxInputVars){
          if (messageExists) {
            $('#find_replace_warning').remove();
            $("form").submit(function () {
              $(this).unbind('submit').submit();
            });
          }
        }

        else {
          if (!messageExists) {
            $('#content.clearfix').prepend(markup);
            $("form").submit(function (e) {
              e.preventDefault();
            });
          }
        }
      }

      $('#edit-process-all').change(function () {
        if (this.checked === true) {
          $('#find_replace_warning').remove();
          $('.form-checkbox.in_tableselect').attr('checked', false);
          $('#selectall.form-checkbox').attr('checked', false);
          $('.select-all').children().attr('checked', false);
          $('tbody').children().removeClass('selected');
        }
      });

      $('.select-all').change(function () {
        $('#edit-process-all').attr('checked', false);
        $('.form-checkbox').tooManySelected();
      });

      $('.form-checkbox.in_tableselect').change(function () {
        if (this.click) {
          $('.form-checkbox').tooManySelected();
        }
        if (this.checked === true) {
          $('#edit-process-all').attr('checked', false);
        }
      });

    }
  }
}(jQuery));
