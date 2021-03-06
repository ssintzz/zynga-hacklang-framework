<?hh //strict

namespace Zynga\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\SInt64Box;
use Zynga\Framework\Type\V1\Test\ValidValue;

class SInt64BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(PHP_INT_MIN);
    $this->setMax(PHP_INT_MAX);
  }

  public function getTypeObject(): TypeInterface {
    return new SInt64Box();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    $values->add(new ValidValue(true, intval(true)));
    $values->add(new ValidValue(false, intval(false)));
    $values->add(new ValidValue(2983.0, intval(2983.0)));
    $values->add(new ValidValue(8974524.0, intval(8974524.0)));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values[] = 3.5;
    $values[] = 4.00123;
    $values[] = -3.5;
    $values[] = -4.00123;
    return $values;
  }
}
