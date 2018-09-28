/**
 * @file
 * check if max_input_vars is met on form.
 */

maxInputVars = Drupal.settings.islandora_find_replace.maxInputVars;
message = `You have selected more than the php variable max_input_vars will allow. Don't select over ${maxInputVars} items in the form!`;
markup = `<div id="console" class="clearfix"><div id="find_replace_warning" class="messages error"><h2 class="element-invisible">Status message</h2><pre>${message}</pre></div></div>`;

(function ($) {
  Drupal.behaviors.tooManyMessage = {
    attach: function (context, settings) {
      $.fn.tooManySelected = function() {
        selected = this.parent().parent().parent('.selected').length;
        messageExists = $('#find_replace_warning').length;

        if (selected < maxInputVars){
          if (messageExists) {
            $('#find_replace_warning').parent().html('');
            $("form").submit(function(){
              $(this).unbind('submit').submit()
            });
          }
        }

        else {
          if(!messageExists){
            $('#content.clearfix').prepend(markup);
            $("form").submit(function(e){
              e.preventDefault();
            });
          }
        }
      }
      $('#edit-process-all').change(function(){
        if(this.checked === true) {
          $('.form-checkbox.in_tableselect').attr('checked', false);
          $('#selectall.form-checkbox').attr('checked', false);
          $('.select-all').children().attr('checked', false);
          $('tbody').children().removeClass('selected');
        }
      });

      $('.select-all').change(function(){
        $('#edit-process-all').attr('checked', false);
        $('.form-checkbox').tooManySelected();
      });

      $('.form-checkbox.in_tableselect').change(function(){
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
