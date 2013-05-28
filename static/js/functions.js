function getFormContents($url, $labelId) {
	$.post($url, {labelId: $labelId}, function(data){
		$data = $.parseJSON(data);
		if($data.length == 0){
			$content = "<input type='text' name='value' />"
		} else {
			$content = "<select name='value'>";
			$.each($data, function(index, value){
				$content = $content + "<option value='"+value+"'>"+value+" ";
			})
			$content = $content + "</select>";
		}
		$inputLabelId = "<input type='hidden' name='labelId' value='"+$labelId+"' />";
		$submit = " <br/><input type='submit' class='btn' value='Save' />";
		$('#labelFormContent').html($content + $inputLabelId + $submit);
	})
}