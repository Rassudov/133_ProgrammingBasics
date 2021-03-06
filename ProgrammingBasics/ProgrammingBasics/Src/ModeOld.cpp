#include "Presenter.h"
#include "Mode.h"


Mode* Mode::UnexpectedEvent(const Event e) {
	switch (e) {
	case ev_createPoint: {
		return new CreatingPoint();
	}
	case ev_createArc: {
		return new CreatingArc();
	}
	case ev_createSegment: {
		return new CreatingSegment();
	}
	case ev_createCircle: {
		return new CreatingCircle();
	}
	case ev_mouseMove:
		return nullptr;
	case ev_leftMouseUp:
		return nullptr;
	case ev_ctrlDown:
		return nullptr;
	case ev_ctrlUp:
		return nullptr;
	default:
		return new Selection();
	}
}

// SEGMENT

CreatingSegment::CreatingSegment() : segmentParameters(4) {
	state = noClick;
}

Mode* CreatingSegment::HandleEvent(const Event ev, Array<double>& params) {
	if (ev == ev_leftMouseDown) {
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		// if it were no clicks
		// then create one point and change the state
		// to one click
		if (state == noClick) {

			segmentParameters[0] = params[0];
			segmentParameters[1] = params[1];

			state = oneClick;
			//for draw mode
			infoMode.x = params[0];
			infoMode.y = params[1];
			return nullptr;
		}

		// if it was one click
		// then create a segment
		// and turn to single selection mode
		// with selected segment
		if (state == oneClick) {

			segmentParameters[2] = params[0];
			segmentParameters[3] = params[1];

			ID id;
			id = Presenter::CreateObject(ot_segment, segmentParameters);

			Array<ID> selectedObjects(1);
			selectedObjects[0] = id;
			return new Selection(selectedObjects);
		}
	}
	if (ev == ev_mouseMove)
	{
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}

		if (state == noClick) {
			return nullptr;
		}

		if (state == oneClick) {
			infoMode.x = params[0];
			infoMode.y = params[1];
			return nullptr;
		}
	}
	return UnexpectedEvent(ev);
}

void CreatingSegment::DrawMode() {
	if (state == oneClick)
	{
		Presenter::GetView()->SetColor(red);
		Presenter::GetView()->DrawPoint(Vector2(segmentParameters[0], segmentParameters[1]));
		Presenter::GetView()->SetColor(yellow);
		Presenter::GetView()->DrawLine(Vector2(segmentParameters[0], segmentParameters[1]), infoMode, points);
	}
}

CreatingSegment::~CreatingSegment() {
	segmentParameters.Clear();
	state = noClick;
}

// POINT

Mode* CreatingPoint::HandleEvent(const Event ev, Array<double>& params) {
	if (ev == ev_leftMouseDown) {
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}

		ID id = Presenter::CreateObject(ot_point, params);
		Array<ID> selectedObjects(1);
		selectedObjects[0] = id;
		return new Selection(selectedObjects);
	}
	return UnexpectedEvent(ev);
}

void CreatingPoint::DrawMode() {
}

// CIRCLE
CreatingCircle::CreatingCircle() : CircleParameters(3) {
	state = noClick;
}

Mode* CreatingCircle::HandleEvent(const Event ev, Array<double>& params) {
	if (ev == ev_leftMouseDown) {
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		// if it were no clicks
		// then create one point and change the state
		// to one click
		if (state == noClick) {

			CircleParameters[0] = params[0];
			CircleParameters[1] = params[1];

			state = oneClick;
			//for draw mode
			infoMode.x = params[0];
			infoMode.y = params[1];
			return nullptr;
		}
		// if it was one click
		// then create a circle
		// and turn to single selection mode
		// with selected circle
		if (state == oneClick) {
			CircleParameters[2] = (Vector2(params[0], params[1]) - Vector2(CircleParameters[0], CircleParameters[1])).GetLength();
			ID id = Presenter::CreateObject(ot_circle, CircleParameters);

			Array<ID> selectedObjects(1);
			selectedObjects[0] = id;
			return new Selection(selectedObjects);
		}
	}
	if (ev == ev_mouseMove)
	{
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		if (state == noClick) {
			return nullptr;
		}

		if (state == oneClick) {
			infoMode.x = params[0];
			infoMode.y = params[1];
			return nullptr;
		}
	}
	return this->UnexpectedEvent(ev);
}

void CreatingCircle::DrawMode() {
	if (state == oneClick)
	{
		Presenter::GetView()->SetColor(red);
		Presenter::GetView()->DrawPoint(Vector2(CircleParameters[0], CircleParameters[1]));
		Presenter::GetView()->SetColor(yellow);
		Presenter::GetView()->DrawCircle(Vector2(CircleParameters[0], CircleParameters[1]), infoMode, points);
	}
}

CreatingCircle::~CreatingCircle() {
	CircleParameters.Clear();
}

// ARC

CreatingArc::CreatingArc() {
	state = noClick;
}

Mode* CreatingArc::HandleEvent(const Event ev, Array<double>& params) {
	if (ev == ev_leftMouseDown) {
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		// if it were no clicks
		// then create one point and change the state
		// to one click
		if (state == noClick) {

			center.x = params[0];
			center.y = params[1];

			state = oneClick;
			//for draw mode
			infoMode.x = params[0];
			infoMode.y = params[1];
			return nullptr;
		}
		// if it were one clicks
		// then create one point and change the state
		// to two click
		if (state == oneClick) {

			point1.x = params[0];
			point1.y = params[1];

			radius = (point1 - center).GetLength();

			state = twoClick;
			return nullptr;
		}
		// if it was two click
		// then create a arc
		// and turn to single selection mode
		// with selected arc
		if (state == twoClick) {
			
			point2.x = params[0];
			point2.y = params[1];

			double radius2 = (point2 - center).GetLength();

			if (abs(radius2) > DBL_EPSILON) {
				point2 = center + (point2 - center) / radius2 * radius;
			}
			else {
				point2 = center - (point1 - center);
			}

			double angle = Vector2::Angle(point1 - center, point2 - center);

			Array<double> objParams = CreateArr(point1.x, point1.y, point2.x, point2.y, angle);

			ID id = Presenter::CreateObject(ot_arc, objParams);

			Array<ID> selectedObjects(1);
			selectedObjects[0] = id;
			return new Selection(selectedObjects);
		}
	}
	if (ev == ev_mouseMove)
	{
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		if (state == noClick) {
			return nullptr;
		}

		if (state == oneClick || state == twoClick) {
			infoMode.x = params[0];
			infoMode.y = params[1];
			return nullptr;
		}
	}
	return this->UnexpectedEvent(ev);
}

void CreatingArc::DrawMode() {
	if (state == oneClick)
	{
		Presenter::GetView()->SetColor(red);
		Presenter::GetView()->DrawPoint(center);

		Presenter::GetView()->SetColor(yellow);
		Presenter::GetView()->DrawCircle(center, infoMode, points);
	}
	if (state == twoClick)
	{
		Presenter::GetView()->SetColor(red);
		Presenter::GetView()->DrawPoint(center);

		Presenter::GetView()->SetColor(yellow);
		Presenter::GetView()->DrawCircle(center, point1, points);
		
		Presenter::GetView()->SetColor(red);
		Presenter::GetView()->DrawPoint(point1);
		
		Presenter::GetView()->SetColor(yellow);
		Presenter::GetView()->DrawArc(center, point1, infoMode, line);
	}
}

CreatingArc::~CreatingArc() {
}

// SELECTION

Selection::Selection(Array<ID> _selObjects) : Mode(), selectedObjects(_selObjects) {
	if (selectedObjects.GetSize() == 0) {
		selectedObjects = Array<ID>(0);
	}
	state = single_selection;
}

Selection::Selection() : Mode(), selectedObjects(0) {
	state = single_selection;
}

Selection::~Selection() {
	selectedObjects.Clear();
}

void Selection::AddObject(const ID& obj) {
	for (int i = 0; i < selectedObjects.GetSize(); ++i) {
		if (selectedObjects[i] == obj) {
			selectedObjects.EraseO_1_(i);
			return;
		}
	}
	selectedObjects.PushBack(obj);
}

Mode* Selection::HandleEvent(const Event e, Array<double>& params) {
	if (e == ev_leftMouseDown) {
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}

		//for area selection
		lastEvent = e;
		infoArea1.x = params[0];
		infoArea1.y = params[1];

		ID obj;
		bool isFound = Presenter::GetObject(params[0], params[1], obj);
		if (isFound) {
			if (state == single_selection) {
				selectedObjects.Clear();
				selectedObjects.PushBack(obj);
				return nullptr;
			}

			if (state == poly_selection) {
				AddObject(obj);
				return nullptr;
			}
		}
		else {
			if (state != poly_selection)
			{
				selectedObjects.Clear();
			}
			return nullptr;
		}
	}

	//for area selection
	if (e == ev_mouseMove && (lastEvent == ev_leftMouseDown || lastEvent == ev_mouseMove) && state != poly_selection)
	{
		state = area_selection;
		infoArea2.x = params[0];
		infoArea2.y = params[1];
		selectedObjects.Clear();
		lastEvent = e;
		Presenter::GetObjectsOnArea(infoArea1.x, infoArea1.y, infoArea2.x, infoArea2.y, selectedObjects);
		return nullptr;
	}
	if (e == ev_leftMouseUp)
	{
		if (lastEvent == ev_mouseMove)
		{
			state = single_selection;
		}
		lastEvent = e;
		return nullptr;
	}
	
	switch (e)
	{
	case ev_ctrlDown: {
		state = poly_selection;
		return nullptr;
	}
	case ev_ctrlUp: {
		state = single_selection;
		return nullptr;
	}
	case ev_altDown: {
		return new NavigationOnScene(selectedObjects);
	}
	case ev_escape: {
		selectedObjects.Clear();
		return nullptr;
	}
	case ev_moveObjects: {
		if (selectedObjects.GetSize() == 0) {
			return nullptr;
		}
		return new Redaction(selectedObjects, ev_moveObjects);
	}
	case ev_scaleObjects: {
		if (selectedObjects.GetSize() == 0) {
			return nullptr;
		}
		return new Redaction(selectedObjects, ev_scaleObjects);
	}
	case ev_del: {
		Presenter::DeletePrimitives(selectedObjects);
		selectedObjects.Clear();
		return nullptr;
	}
	case ev_req_D_point: {
		return new CreateRequirementWithParam(selectedObjects, e);
	}
	case ev_req_D_point_segment: {
		return new CreateRequirementWithParam(selectedObjects, e);
	}
	case ev_req_D_point_arc: {
		return new CreateRequirementWithParam(selectedObjects, e);
	}
	case ev_req_angle_segment: {
		return new CreateRequirementWithParam(selectedObjects, e);
	}
	case ev_req_Eq_Segment: {
		Array<double>param(0);

		Presenter::CreateRequirement(ot_equalSegmentLen, selectedObjects, param);
		return nullptr;
	}
	case ev_req_on_one_hand: {
		Array<double>param(0);

		Presenter::CreateRequirement(ot_pointsOnTheOneHand, selectedObjects, param);
		return nullptr;
	}
	default:
		return UnexpectedEvent(e);
	}
}

void Selection::DrawMode()
{
	Presenter::DrawSelectedObjects(selectedObjects);

	if (state == area_selection)
	{
		Vector2 point1(infoArea2.x, infoArea1.y);
		Vector2 point2(infoArea1.x, infoArea2.y);

		Presenter::GetView()->SetColor(blue);
		Presenter::GetView()->DrawLine(infoArea1, point1, points);
		Presenter::GetView()->DrawLine(infoArea1, point2, points);
		Presenter::GetView()->DrawLine(infoArea2, point1, points);
		Presenter::GetView()->DrawLine(infoArea2, point2, points);
	}
}

// REDACTION

Redaction::Redaction(Array<ID> _selecObj, Event _ev) : selectedObjects(_selecObj){
	state = noClick;
	switch (_ev)
	{
	case ev_moveObjects:
		status = move;
		break;
	case ev_scaleObjects:
		status = scale;
		break;
	default:
        std::invalid_argument("Redaction : not valid status");
		break;
	}
}

Redaction::~Redaction() {
	selectedObjects.Clear();
}

Mode* Redaction::HandleEvent(const Event e, Array<double>& params)
{
	if (status == move)
	{
		if (e == ev_leftMouseDown) {
			if (params.GetSize() != 2) {
                throw std::invalid_argument("Bad number of parameters");
			}
			posStart.x = params[0];
			posStart.y = params[1];
			state = click;
			posEnd = posStart;
			return nullptr;
		}
		if (e == ev_mouseMove && state == click)
		{
			if (params.GetSize() != 2) {
				throw std::invalid_argument("Bad number of parameters");
			}
			posEnd.x = params[0];
			posEnd.y = params[1];
			Presenter::MoveObject(selectedObjects, posEnd - posStart);
			posStart = posEnd;
			return nullptr;
		}
		if (e == ev_leftMouseUp) {
			state = noClick;
			return nullptr;
		}
	}
	if (status == scale)
	{
		if (e == ev_scroll)
		{
			if (params.GetSize() != 1) {
                throw std::invalid_argument("Bad number of parameters");
			}
			double coef;
			if (params[0] > 0 )
			{
				coef = 0.9;
			}
			if (params[0] < 0)
			{
				coef = 1.1;
			}
			Presenter::ScaleObjects(selectedObjects, coef);
			return nullptr;
		}
	}
	if (e == ev_escape)
	{
		return new Selection(selectedObjects);
	}
	if (e == ev_scaleObjects)
	{
		return new Redaction(selectedObjects, ev_scaleObjects);
	}
	if (e == ev_moveObjects)
	{
		return new Redaction(selectedObjects, ev_moveObjects);
	}
	if (e == ev_del)
	{
		Presenter::DeletePrimitives(selectedObjects);
		return new Selection();
	}
	return UnexpectedEvent(e);
}

void Redaction::DrawMode() {
	Presenter::DrawSelectedObjects(selectedObjects);
}

// REDACTION_REQ

RedactionReq::RedactionReq(ID _selecObj) : selectedPrim(_selecObj) {
	Presenter::GetComponent(selectedPrim, objects, reqs);
}

RedactionReq::RedactionReq() { }

RedactionReq::~RedactionReq() {
	objects.Clear();
	reqs.Clear();
	objectsOfreq.Clear();
}

Mode* RedactionReq::HandleEvent(const Event ev, Array<double>& param) {
	return nullptr;
}

void RedactionReq::DrawMode() {

}


//CREATE REQUIRMENT
CreateRequirementWithParam::CreateRequirementWithParam(Array<ID> _selecObj, Event _ev) : selectedPrim(_selecObj) {
	switch (_ev)
	{
	case ev_req_D_point: {
		typeRequirement = ot_distBetPoints;
		break;
	}
	case ev_req_D_point_segment: {
		typeRequirement = ot_distBetPointSeg;
		break;
	}
	case ev_req_D_point_arc: {
		typeRequirement = ot_distBetPointArc;
		break;
	}
	case ev_req_angle_segment: {
		typeRequirement = ot_angleBetSeg;
		break;
	}
	default:
        std::invalid_argument("CreateRequirement : not valid status");
		break;
	}
}

CreateRequirementWithParam::CreateRequirementWithParam() { }

CreateRequirementWithParam::~CreateRequirementWithParam() {
	
}

Mode* CreateRequirementWithParam::HandleEvent(const Event ev, Array<double>& params) {
	if (ev == ev_input)
	{
		if (params.GetSize() != 1) {
            throw std::invalid_argument("Bad number of parameters");
		}

		Presenter::CreateRequirement(typeRequirement, selectedPrim, params);
		return new Selection(selectedPrim);

	}
	if (ev == ev_escape)
	{
		return new Selection(selectedPrim);
	}

	return UnexpectedEvent(ev);
}

void CreateRequirementWithParam::DrawMode() {
	Presenter::DrawSelectedObjects(selectedPrim);
}

//NAVIGATION ON SCENE
NavigationOnScene::NavigationOnScene(Array<ID> _selecObj) : selectedPrim(_selecObj) { 
	stateMove =  noClick;
	speedMove = 10;
}

NavigationOnScene::NavigationOnScene() { }

NavigationOnScene::~NavigationOnScene() { }

Mode* NavigationOnScene::HandleEvent(const Event ev, Array<double>& params) {
	//for translate 
	if (ev == ev_leftMouseDown) {
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		posStart.x = params[0];
		posStart.y = params[1];
		stateMove = click;
		posEnd = posStart;
		return nullptr;
	}
	if (ev == ev_mouseMove && stateMove == click)
	{
		if (params.GetSize() != 2) {
            throw std::invalid_argument("Bad number of parameters");
		}
		posEnd.x = params[0];
		posEnd.y = params[1];
		Presenter::GetView()->TranslateScene(posEnd - posStart);
		posStart = posEnd;
		return nullptr;
	}
	if (ev == ev_leftMouseUp) {
		stateMove = noClick;
		return nullptr;
	}

	if (ev == ev_arrowUp)
	{
		Presenter::GetView()->TranslateScene(Vector2(0, -speedMove));
		return nullptr;
	}
	if (ev == ev_arrowDown)
	{
		Presenter::GetView()->TranslateScene(Vector2(0, speedMove));
		return nullptr;
	}
	if (ev == ev_arrowLeft)
	{
		Presenter::GetView()->TranslateScene(Vector2(-speedMove, 0));
		return nullptr;
	}
	if (ev == ev_arrowRight)
	{
		Presenter::GetView()->TranslateScene(Vector2(speedMove, 0));
		return nullptr;
	}
	
	//for scale
	if (ev == ev_scroll)
	{
		if (params.GetSize() != 1) {
            throw std::invalid_argument("Bad number of parameters");
		}
		double deltaCoef;
		if (params[0] > 0)
		{
			deltaCoef = -0.1;
		}
		if (params[0] < 0)
		{
			deltaCoef = 0.1;
		}
		Presenter::GetView()->ScaleScene(deltaCoef);
		return nullptr;
	}

	//for rotate
	//..

	//exit
	if (ev == ev_altUp)
	{
		return new Selection(selectedPrim);
	}

	return UnexpectedEvent(ev);
}

void NavigationOnScene::DrawMode() {
	Presenter::DrawSelectedObjects(selectedPrim);
}
