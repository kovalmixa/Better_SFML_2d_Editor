#include <algorithm>
#include <ranges>
#include <cmath>

#include "basic_functions.h"
#include "selection_controller.h"
#include "logic_controller.h"

SelectionController* SelectionController::instance_ = nullptr;

SelectionController::SelectionController()
{
	selection_rect_ = sf::RectangleShape();
	selection_rect_.setFillColor(sf::Color::Transparent);
	selection_rect_.setOutlineColor(sf::Color::Blue);
	selection_rect_.setOutlineThickness(1.f);

	transform_points_[TransformMode::Angle] = new sf::ConvexShape(3);
	auto triangle = dynamic_cast<sf::ConvexShape*>(transform_points_[TransformMode::Angle]);
	triangle->setPoint(0, { TRANSFORM_POINTS_SIZE, TRANSFORM_POINTS_SIZE });
	triangle->setPoint(1, { TRANSFORM_POINTS_SIZE / 2 , 0 });
	triangle->setPoint(2, { 0, TRANSFORM_POINTS_SIZE });
	triangle->scale({ 2, 2 });

	transform_points_[TransformMode::Center] = new sf::RectangleShape({ TRANSFORM_POINTS_SIZE, TRANSFORM_POINTS_SIZE });
	transform_points_[TransformMode::Center]->setRotation(sf::degrees(45.f));
	transform_points_[TransformMode::Center]->scale({ 2, 2 });

	transform_points_[TransformMode::TopLeft] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Left] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::BottomLeft] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::TopRight] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Right] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::BottomRight] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Top] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);
	transform_points_[TransformMode::Bottom] = new sf::CircleShape(TRANSFORM_POINTS_SIZE / 2.f);

	for (auto point : transform_points_) {
		point.second->setFillColor(sf::Color::White);
		point.second->setOutlineColor(sf::Color::Blue);
		point.second->setOutlineThickness(1.f);
	}
}

SelectionController::~SelectionController()
{
	for (auto transform_point : transform_points_)
		delete transform_point.second;
	transform_points_.clear();
}

void SelectionController::remove_shape_from_selection(BaseShape* shape)
{
	if (!shape) return;
	selected_shapes_.remove(shape);
	is_selection_active_ = !(selected_shapes_.empty());
	if (is_selection_active_) {
		update_transform_data();
		update_transform_points();
	}
	else clear_selection();
}

void SelectionController::update_transform_data()
{
	if (selected_shapes_.empty()) return;
	selection_transform_data = get_all_shapes_bounds_transform(selected_shapes_);
	selection_rect_.setSize(selection_transform_data.size);
	selection_rect_.setOrigin(selection_transform_data.size / 2.f);
	selection_rect_.setPosition(selection_transform_data.position);
	selection_rect_.setRotation(selection_transform_data.rotation);
}

void SelectionController::update_transform_points()
{
	float centerX = selection_transform_data.position.x;
	float centerY = selection_transform_data.position.y;
	float halfW = selection_transform_data.size.x / 2.f;
	float halfH = selection_transform_data.size.y / 2.f;

	auto localTopLeft = sf::Vector2f(-halfW, -halfH);
	auto localLeft = sf::Vector2f(-halfW, 0.f);
	auto localBottomLeft = sf::Vector2f(-halfW, halfH);
	auto localTopRight = sf::Vector2f(halfW, -halfH);
	auto localRight = sf::Vector2f(halfW, 0.f);
	auto localBottomRight = sf::Vector2f(halfW, halfH);
	auto localTop = sf::Vector2f(0.f, -halfH);
	auto localBottom = sf::Vector2f(0.f, halfH);

	selection_rect_.setPosition(selection_transform_data.position);
	selection_rect_.setSize(selection_transform_data.size);
	selection_rect_.setRotation(selection_transform_data.rotation);
	set_pivot_center_to_shape(selection_rect_);

	float angleRad = selection_transform_data.rotation.asRadians();
	float cosA = std::cos(angleRad);
	float sinA = std::sin(angleRad);

	auto rotate_local_to_world = [&](const sf::Vector2f& localOffset) -> sf::Vector2f {
		return sf::Vector2f(
			centerX + (localOffset.x * cosA - localOffset.y * sinA),
			centerY + (localOffset.x * sinA + localOffset.y * cosA)
		);
		};

	for (auto transform_point : transform_points_) {
		auto shape = transform_point.second;
		set_pivot_center_to_shape(*shape);

		switch (transform_point.first)
		{
		case TransformMode::Angle:
			shape->setPosition(rotate_local_to_world(sf::Vector2f(0.f, -halfH - 42.f)));
			break;
		case TransformMode::Center:
			shape->setPosition(rotate_local_to_world(sf::Vector2f(0.f, 0.f)));
			break;
		case TransformMode::TopLeft:
			shape->setPosition(rotate_local_to_world(localTopLeft));
			break;
		case TransformMode::Left:
			shape->setPosition(rotate_local_to_world(localLeft));
			break;
		case TransformMode::BottomLeft:
			shape->setPosition(rotate_local_to_world(localBottomLeft));
			break;
		case TransformMode::TopRight:
			shape->setPosition(rotate_local_to_world(localTopRight));
			break;
		case TransformMode::Right:
			shape->setPosition(rotate_local_to_world(localRight));
			break;
		case TransformMode::BottomRight:
			shape->setPosition(rotate_local_to_world(localBottomRight));
			break;
		case TransformMode::Top:
			shape->setPosition(rotate_local_to_world(localTop));
			break;
		case TransformMode::Bottom:
			shape->setPosition(rotate_local_to_world(localBottom));
			break;
		default: break;
		}
		shape->setRotation(selection_transform_data.rotation);
	}
}

bool SelectionController::is_point_near_transform_point(sf::Vector2f point, sf::Shape* transform_point)
{
	return euclidean_distance(point, transform_point->getPosition()) <= TRANSFORM_POINTS_DRAG_RADIUS_OFFSET; 
}

void SelectionController::drag_rotation(sf::Vector2f mouse_position)
{
	auto center = selection_transform_data.position;
	auto vector_to_mouse = get_vector_from_points(center, mouse_position);
	auto vector_to_rotation_point = get_vector_from_points(center, transform_points_[TransformMode::Angle]->getPosition());
	float angle = angle_between_vectors_2d(vector_to_rotation_point, vector_to_mouse);
	selection_transform_data.rotation += sf::radians(angle);
	update_transform_to_selected_shapes();
}

void SelectionController::drag_movement(sf::Vector2f mouse_position)
{
	auto delta = get_vector_from_points(selection_transform_data.position, mouse_position);
	selection_transform_data.position += delta;
	update_transform_to_selected_shapes();
}

void SelectionController::drag_resize(sf::Vector2f mouse_position, TransformMode trans_mode)
{
	float angle = selection_transform_data.rotation.asRadians();

	if (std::abs(angle) > 1e-6f && selected_shapes_.size() == 1) {
		auto world_to_local = [&](const sf::Vector2f& world) -> sf::Vector2f {
			sf::Vector2f rel = world - selection_transform_data.position;
			float cosA = std::cos(-angle);
			float sinA = std::sin(-angle);
			return sf::Vector2f(rel.x * cosA - rel.y * sinA, rel.x * sinA + rel.y * cosA);
			};
		auto local_to_world = [&](const sf::Vector2f& local) -> sf::Vector2f {
			float cosA = std::cos(angle);
			float sinA = std::sin(angle);
			return selection_transform_data.position + sf::Vector2f(local.x * cosA - local.y * sinA, local.x * sinA + local.y * cosA);
			};

		sf::Vector2f half = selection_transform_data.size / 2.f;
		sf::Vector2f leftTopLocal = -half;
		sf::Vector2f rightBottomLocal = half;

		sf::Vector2f mouseLocal = world_to_local(mouse_position);

		switch (trans_mode) {
		case TransformMode::TopLeft: leftTopLocal = mouseLocal; break;
		case TransformMode::TopRight: rightBottomLocal.x = mouseLocal.x; leftTopLocal.y = mouseLocal.y; break;
		case TransformMode::BottomLeft: rightBottomLocal.y = mouseLocal.y; leftTopLocal.x = mouseLocal.x; break;
		case TransformMode::BottomRight: rightBottomLocal = mouseLocal; break;
		case TransformMode::Left: leftTopLocal.x = mouseLocal.x; break;
		case TransformMode::Right: rightBottomLocal.x = mouseLocal.x; break;
		case TransformMode::Top: leftTopLocal.y = mouseLocal.y; break;
		case TransformMode::Bottom: rightBottomLocal.y = mouseLocal.y; break;
		default: return;
		}

		sf::Vector2f minCornerLocal{ std::min(leftTopLocal.x, rightBottomLocal.x), std::min(leftTopLocal.y, rightBottomLocal.y) };
		sf::Vector2f maxCornerLocal{ std::max(leftTopLocal.x, rightBottomLocal.x), std::max(leftTopLocal.y, rightBottomLocal.y) };
		sf::Vector2f newSizeLocal = maxCornerLocal - minCornerLocal;
		sf::Vector2f newCenterLocal = (minCornerLocal + maxCornerLocal) / 2.f;

		if (newSizeLocal.x < 1.f) newSizeLocal.x = 1.f;
		if (newSizeLocal.y < 1.f) newSizeLocal.y = 1.f;

		selection_transform_data.size = newSizeLocal;
		selection_transform_data.position = local_to_world(newCenterLocal);
		update_transform_to_selected_shapes();
		return;
	}

	sf::Vector2f half = selection_transform_data.size / 2.f;
	sf::Vector2f leftTop = selection_transform_data.position - half;
	sf::Vector2f rightBottom = selection_transform_data.position + half;

	sf::Vector2f newLT = leftTop;
	sf::Vector2f newRB = rightBottom;

	switch (trans_mode) {
	case TransformMode::TopLeft: newLT = mouse_position; break;
	case TransformMode::TopRight: newRB.x = mouse_position.x; newLT.y = mouse_position.y; break;
	case TransformMode::BottomLeft: newRB.y = mouse_position.y; newLT.x = mouse_position.x; break;
	case TransformMode::BottomRight: newRB = mouse_position; break;
	case TransformMode::Left: newLT.x = mouse_position.x; break;
	case TransformMode::Right: newRB.x = mouse_position.x; break;
	case TransformMode::Top: newLT.y = mouse_position.y; break;
	case TransformMode::Bottom: newRB.y = mouse_position.y; break;
	default: return;
	}

	sf::Vector2f minCorner{ std::min(newLT.x, newRB.x), std::min(newLT.y, newRB.y) };
	sf::Vector2f maxCorner{ std::max(newLT.x, newRB.x), std::max(newLT.y, newRB.y) };
	sf::Vector2f newSize = maxCorner - minCorner;
	sf::Vector2f newCenter = (minCorner + maxCorner) / 2.f;

	if (newSize.x < 1.f) newSize.x = 1.f;
	if (newSize.y < 1.f) newSize.y = 1.f;

	selection_transform_data.size = newSize;
	selection_transform_data.position = newCenter;
	update_transform_to_selected_shapes();
}

void SelectionController::drag_selection(sf::Vector2f mouse_position)
{
}

void SelectionController::update_transform_to_selected_shapes()
{
	auto lastCenter = last_transform_data.position;
	auto newCenter = selection_transform_data.position;
	auto lastSize = last_transform_data.size;
	auto newSize = selection_transform_data.size;

	auto deltaRotation = selection_transform_data.rotation - last_transform_data.rotation;
	if (selected_shapes_.size() == 1) {
		auto* shape = selected_shapes_.front();
		TransformData newTransform;
		newTransform.position = selection_transform_data.position;
		newTransform.size = selection_transform_data.size;
		newTransform.rotation = selection_transform_data.rotation;
		shape->set_transform(newTransform);
		update_transform_points();
		return;
	}

	float scaleX = lastSize.x != 0.f ? (newSize.x / lastSize.x) : 1.f;
	float scaleY = lastSize.y != 0.f ? (newSize.y / lastSize.y) : 1.f;
	for (auto* shape : selected_shapes_)
	{
		auto shape_transform = shape->get_transform();
		sf::Vector2f offset = shape_transform.position - lastCenter;
		sf::Vector2f scaledOffset{ offset.x * scaleX, offset.y * scaleY };
		sf::Vector2f scaledSize{ shape_transform.size.x * scaleX, shape_transform.size.y * scaleY };

		shape_transform.position = newCenter + scaledOffset;
		shape_transform.size = scaledSize;
		shape_transform.rotation += deltaRotation;

		shape->set_transform(shape_transform);
	}
	update_transform_points();
}

TransformData SelectionController::get_all_shapes_bounds_transform(std::list<BaseShape*> shapes)
{
	if (shapes.empty()) return TransformData::Default;
	if (shapes.size() == 1) return shapes.front()->get_transform();

	float min_x = std::numeric_limits<float>::max();
	float min_y = std::numeric_limits<float>::max();
	float max_x = std::numeric_limits<float>::lowest();
	float max_y = std::numeric_limits<float>::lowest();

	for (auto* shape : shapes)
	{
		auto bounds = shape->get_global_bounds();
		min_x = std::min(min_x, bounds.position.x);
		min_y = std::min(min_y, bounds.position.y);
		max_x = std::max(max_x, bounds.position.x + bounds.size.x);
		max_y = std::max(max_y, bounds.position.y + bounds.size.y);
	}
	float width = max_x - min_x;
	float height = max_y - min_y;
	sf::Vector2f center = { min_x + width / 2.f, min_y + height / 2.f };

	return { center, {width, height}, sf::degrees(0.f) };
}

SelectionController* SelectionController::get_instance() 
{ 
	return (instance_ == nullptr) ? instance_ = new SelectionController() : instance_;
}

std::list<BaseShape*> SelectionController::get_selected_shapes() { return selected_shapes_; }

bool SelectionController::is_point_on_selection(sf::Vector2f point)
{
	if (!is_selection_active_) return false;
	for (auto& tp : transform_points_)
		if (is_point_near_transform_point(point, tp.second)) return true;
	if (selection_rect_.getGlobalBounds().contains(point)) return true;
	return false;
}

bool SelectionController::try_select_shape(BaseShape* shape, sf::Vector2f point, bool is_union)
{
	if (!shape->contains(point)) return false;
	try_add_shape_to_selection(shape, is_union);
	return true;
}

void SelectionController::try_add_shape_to_selection(BaseShape* shape, bool is_union)
{
	if (is_transforming_) return;

	if (!is_union) clear_selection();
	bool is_selected = std::find(selected_shapes_.begin(), selected_shapes_.end(), shape) != selected_shapes_.end();
	if (!is_selected) selected_shapes_.push_back(shape);
	else {
		selected_shapes_.remove(shape);
		shape->set_outline(false);
	}
	is_selection_active_ = !(selected_shapes_.size() == 0);
	if (is_selection_active_) {
		update_transform_data();
		update_transform_points();
	}
	for (auto& shape : selected_shapes_)
		shape->set_outline(is_selection_active_);
}

void SelectionController::delete_selected_shapes()
{
	std::vector<BaseShape*> toDelete(selected_shapes_.begin(), selected_shapes_.end());
	for (auto* shape : toDelete) 
		if (shape) LogicController::get_instance()->delete_shape(shape);
	clear_selection();
}

void SelectionController::clear_selection()
{
	for (auto& shape : selected_shapes_)
		if (shape) shape->set_outline(false);
	selected_shapes_.clear();
	is_selection_active_ = false;
}

void SelectionController::try_begin_drag_transform_mode(sf::Vector2f mouse_position)
{
	if (!is_selection_active_) return;
	for (auto trans_point : transform_points_)
	{
		if (is_point_near_transform_point(mouse_position, trans_point.second))
		{
			active_transform_mode_ = trans_point.first;
			is_transforming_ = true;
			return;
		}
	}
	if (selection_rect_.getGlobalBounds().contains(mouse_position))
		active_transform_mode_ = TransformMode::Center;
	else active_transform_mode_ = TransformMode::None;
}

void SelectionController::end_drag_transform_mode()
{
	is_transforming_ = false;
	active_transform_mode_ = TransformMode::None;
}

void SelectionController::update_transform(sf::Vector2f mouse_position)
{
	if (!is_selection_active_ || !is_transforming_) return;
	last_transform_data = selection_transform_data;
	switch (active_transform_mode_)
	{
	case TransformMode::Angle: drag_rotation(mouse_position); break;
	case TransformMode::Center: drag_movement(mouse_position); break;
	default: drag_resize(mouse_position, active_transform_mode_); break;
	}
}

void SelectionController::try_copy_shapes()
{
	if (!is_selection_active_) return;
	for (auto shape : copy_buffer) delete shape;
	copy_buffer.clear();

	for (auto shape : selected_shapes_)
		if (shape) copy_buffer.push_back(shape->clone());
}

void SelectionController::try_paste_shapes(sf::Vector2f mouse_position)
{
	if (copy_buffer.empty()) return;
	clear_selection();
	auto copy_transform = get_all_shapes_bounds_transform(copy_buffer);
	auto copy_center = copy_transform.position;
	std::vector<BaseShape*> pasted;
	for (auto& proto : copy_buffer) {
		if (!proto) continue;
		auto new_shape = proto->clone();
		auto shape_transform = new_shape->get_transform();
		auto offset = shape_transform.position - copy_center;
		new_shape->set_transform(mouse_position + offset, shape_transform.size, shape_transform.rotation);
		LogicController::get_instance()->add_shape(new_shape);
		pasted.push_back(new_shape);
	}

	if (!pasted.empty()) {
		clear_selection();
		for (auto* s : pasted) try_add_shape_to_selection(s, true);
	}
}

void SelectionController::draw_selection(sf::RenderWindow& window)
{
	if (is_selection_active_) {
		window.draw(selection_rect_);
		for (auto transform_point : transform_points_)
			window.draw(*transform_point.second);
	}
}