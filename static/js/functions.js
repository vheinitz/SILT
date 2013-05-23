function getFormContents($url, $lableId) {
	$.post($url, {lableId: $lableId}, function(data){
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
		$inputLableId = "<input type='hidden' name='lableId' value='"+$lableId+"' />";
		$submit = " <br/><input type='submit' class='btn' value='Save' />";
		$('#lableFormContent').html($content + $inputLableId + $submit);
	})
}